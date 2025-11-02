// manet-jerarquica-opA.cc
// Implementación MANET jerárquica (Opción A) - Líderes patrullan (movilidad dinámica)
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/wifi-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/olsr-helper.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/netanim-module.h"
#include "ns3/ipv4-flow-classifier.h"
#include "ns3/ipv4-static-routing-helper.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cmath>
#include <sstream>
#include <sys/stat.h>
#include <cstdlib>

using namespace ns3;
NS_LOG_COMPONENT_DEFINE("ManetHierarchical_OpA");

// ----- Config structs -----
struct ClusterConfig {
    uint32_t clusterId;
    Vector leaderStartPos;
    uint32_t numNodes;
    double mobilityRadius;
    double subordinateSpeed;
    std::string trajectoryType;
};

struct SimulationParams {
    uint32_t numClusters;
    double simulationTime;
    double leaderSpeed;
    double interClusterDistance;
    std::vector<ClusterConfig> clusters;
};

struct ConnectivityMetrics {
    uint32_t totalChecks;
    uint32_t connectedChecks;
    std::map<std::pair<uint32_t, uint32_t>, uint32_t> pairConnectivity;
    ConnectivityMetrics() : totalChecks(0), connectedChecks(0) {}
    double GetConnectivityRatio() const {
        return totalChecks > 0 ? (double)connectedChecks / totalChecks : 0.0;
    }
};

ConnectivityMetrics globalConnectivity;

// --- Crear directorio de salida junto al .cc (similar a manet-proyect-batman.cc) ---
static std::string
CreateOutputDirectory (const std::string &suffix = "")
{
    std::string sourceFile = __FILE__;
    size_t lastSlash = sourceFile.find_last_of("/\\");
    std::string sourceDir = (lastSlash != std::string::npos) ? sourceFile.substr(0, lastSlash) : ".";
    std::string outDir = sourceDir + "/manet-jerarquica";
    if (!suffix.empty()) outDir += "-" + suffix;
    std::string mkdirCmd = "mkdir -p " + outDir;
    (void)system(mkdirCmd.c_str());
    NS_LOG_UNCOND("Directorio de salida: " << outDir);
    return outDir;
}

// ----- Custom mobility model for leaders -----
class LeaderTrajectoryMobilityModel : public MobilityModel
{
public:
    static TypeId GetTypeId()
    {
        static TypeId tid = TypeId("LeaderTrajectoryMobilityModel")
            .SetParent<MobilityModel>()
            .SetGroupName("Mobility")
            .AddConstructor<LeaderTrajectoryMobilityModel>();
        return tid;
    }

    LeaderTrajectoryMobilityModel() : m_trajectoryType("circular"), m_speed(5.0), m_currentWaypoint(0)
    {
        m_position = Vector(0,0,0);
        m_velocity = Vector(0,0,0);
    }

    void SetTrajectory(const std::string &type, const Vector &center, double radius, double speed)
    {
        m_trajectoryType = type;
        m_center = center;
        m_radius = radius;
        m_speed = speed;
        m_position = center;
        GenerateWaypoints();
        if (!m_waypoints.empty()) {
            m_position = m_waypoints[0];
            ScheduleNextWaypoint();
        }
    }

    virtual Ptr<MobilityModel> Copy() const override {
        Ptr<LeaderTrajectoryMobilityModel> copy = CreateObject<LeaderTrajectoryMobilityModel>();
        copy->m_trajectoryType = m_trajectoryType;
        copy->m_center = m_center;
        copy->m_radius = m_radius;
        copy->m_speed = m_speed;
        copy->m_position = m_position;
        copy->m_velocity = m_velocity;
        copy->m_waypoints = m_waypoints;
        copy->m_currentWaypoint = m_currentWaypoint;
        return copy;
    }

private:
    void GenerateWaypoints()
    {
        m_waypoints.clear();
        if (m_trajectoryType == "circular") {
            int n = 12;
            for (int i=0;i<n;i++) {
                double ang = 2.0*M_PI*i/n;
                m_waypoints.push_back(Vector(m_center.x + m_radius*cos(ang),
                                             m_center.y + m_radius*sin(ang), 0.0));
            }
        } else if (m_trajectoryType == "linear") {
            m_waypoints.push_back(Vector(m_center.x - m_radius, m_center.y, 0));
            m_waypoints.push_back(Vector(m_center.x + m_radius, m_center.y, 0));
        } else if (m_trajectoryType == "rectangular") {
            double h = m_radius * 0.7;
            m_waypoints.push_back(Vector(m_center.x - h, m_center.y - h, 0));
            m_waypoints.push_back(Vector(m_center.x + h, m_center.y - h, 0));
            m_waypoints.push_back(Vector(m_center.x + h, m_center.y + h, 0));
            m_waypoints.push_back(Vector(m_center.x - h, m_center.y + h, 0));
        } else { // zigzag default
            int seg = 4;
            double seglen = (2.0*m_radius)/seg;
            for (int i=0;i<=seg;i++) {
                double x = m_center.x - m_radius + i*seglen;
                double y = m_center.y + ((i%2==0)? -m_radius*0.3 : m_radius*0.3);
                m_waypoints.push_back(Vector(x,y,0));
            }
        }
    }

    void ScheduleNextWaypoint()
    {
        if (m_waypoints.empty()) return;
        Vector next = m_waypoints[m_currentWaypoint];
        double dist = CalculateDistance(m_position, next);
        double t = dist / m_speed;
        if (t > 0) {
            m_velocity.x = (next.x - m_position.x)/t;
            m_velocity.y = (next.y - m_position.y)/t;
            Simulator::Schedule(Seconds(t), &LeaderTrajectoryMobilityModel::ReachWaypoint, this);
        }
    }

    void ReachWaypoint()
    {
        m_position = m_waypoints[m_currentWaypoint];
        m_currentWaypoint = (m_currentWaypoint + 1) % m_waypoints.size();
        NotifyCourseChange();
        ScheduleNextWaypoint();
    }

    virtual Vector DoGetPosition() const override { return m_position; }
    virtual void DoSetPosition(const Vector &position) override { m_position = position; NotifyCourseChange(); }
    virtual Vector DoGetVelocity() const override { return m_velocity; }

    Vector m_position;
    Vector m_velocity;
    Vector m_center;
    double m_radius;
    std::string m_trajectoryType;
    double m_speed;
    std::vector<Vector> m_waypoints;
    uint32_t m_currentWaypoint;
};

NS_OBJECT_ENSURE_REGISTERED(LeaderTrajectoryMobilityModel);

// ----- Helper functions -----
void SetupLeaderMobility(NodeContainer &leaders, const std::vector<ClusterConfig> &clusters, double speed)
{
    for (uint32_t i=0;i<leaders.GetN();++i) {
        Ptr<Node> n = leaders.Get(i);
        Ptr<LeaderTrajectoryMobilityModel> model = CreateObject<LeaderTrajectoryMobilityModel>();
        uint32_t idx = i % clusters.size();
        model->SetTrajectory(clusters[idx].trajectoryType, clusters[idx].leaderStartPos, clusters[idx].mobilityRadius, speed);
        n->AggregateObject(model);
    }
    NS_LOG_INFO("SetupLeaderMobility: líderes configurados");
}

bool CheckConnectivity(Ptr<Node> a, Ptr<Node> b, double maxRange)
{
    Ptr<MobilityModel> ma = a->GetObject<MobilityModel>();
    Ptr<MobilityModel> mb = b->GetObject<MobilityModel>();
    if (!ma || !mb) return false;
    return CalculateDistance(ma->GetPosition(), mb->GetPosition()) <= maxRange;
}

void MonitorSecondLevelClusterConnectivity(NodeContainer leaders, double maxRange)
{
    globalConnectivity.totalChecks++;
    uint32_t connectedPairs = 0;
    uint32_t totalPairs = 0;
    for (uint32_t i=0;i<leaders.GetN();++i) {
        for (uint32_t j=i+1;j<leaders.GetN();++j) {
            totalPairs++;
            std::pair<uint32_t,uint32_t> key(i,j);
            if (CheckConnectivity(leaders.Get(i), leaders.Get(j), maxRange)) {
                connectedPairs++;
                globalConnectivity.pairConnectivity[key]++;
            }
        }
    }
    if (totalPairs>0 && (double)connectedPairs/totalPairs >= 0.5) globalConnectivity.connectedChecks++;
}

void ScheduleConnectivityMonitoring(NodeContainer leaders, double interval, double totalTime, double maxRange)
{
    for (double t=0.0; t<=totalTime; t+=interval) {
        Simulator::Schedule(Seconds(t), &MonitorSecondLevelClusterConnectivity, leaders, maxRange);
    }
}

// Subordinate mobility: RandomWalk centered near leader initially
void SetupSubordinateMobility(NodeContainer &subs, Ptr<Node> leader, double radius, double speed)
{
    MobilityHelper mh;
    Ptr<MobilityModel> lm = leader->GetObject<MobilityModel>();
    Vector center = lm ? lm->GetPosition() : Vector(0,0,0);

    Ptr<ListPositionAllocator> pos = CreateObject<ListPositionAllocator>();
    Ptr<UniformRandomVariable> ur = CreateObject<UniformRandomVariable>();
    for (uint32_t i=0;i<subs.GetN();++i) {
        double x = center.x + ur->GetValue(-radius*0.5, radius*0.5);
        double y = center.y + ur->GetValue(-radius*0.5, radius*0.5);
        pos->Add(Vector(x,y,0));
    }
    mh.SetPositionAllocator(pos);
    mh.SetMobilityModel("ns3::RandomWalk2dMobilityModel",
                        "Bounds", RectangleValue(Rectangle(center.x - radius, center.x + radius,
                                                          center.y - radius, center.y + radius)),
                        "Distance", DoubleValue(radius*0.2),
                        "Speed", StringValue("ns3::ConstantRandomVariable[Constant=" + std::to_string(speed) + "]"));
    mh.Install(subs);
    NS_LOG_INFO("SetupSubordinateMobility: " << subs.GetN() << " subordinados (radio=" << radius << ", speed=" << speed << ")");
}

// Reposition subordinate nodes periodically around their leader so cluster moves with leader
void RepositionSubordinates(NodeContainer subs, Ptr<Node> leader, double radius)
{
    Ptr<MobilityModel> lm = leader->GetObject<MobilityModel>();
    Vector lp = lm ? lm->GetPosition() : Vector(0,0,0);
    Ptr<UniformRandomVariable> ur = CreateObject<UniformRandomVariable>();
    for (uint32_t i=0;i<subs.GetN();++i) {
        Ptr<Node> s = subs.Get(i);
        Ptr<MobilityModel> sm = s->GetObject<MobilityModel>();
        if (!sm) continue;
        double ang = ur->GetValue(0, 2*M_PI);
        double r = ur->GetValue(0, radius*0.8);
        sm->SetPosition(Vector(lp.x + r*cos(ang), lp.y + r*sin(ang), 0));
    }
    Simulator::Schedule(Seconds(1.0), &RepositionSubordinates, subs, leader, radius);
}

// Inter-cluster traffic between leaders (backhaul IPs)
void SetupInterClusterTraffic(NodeContainer &leaders, Ipv4InterfaceContainer &leaderBackhaulIfaces, double startTime, double stopTime)
{
    if (leaders.GetN() < 2) return;
    uint32_t port = 9000;
    for (uint32_t i=0;i<leaders.GetN();++i) {
        for (uint32_t j=i+1;j<leaders.GetN();++j) {
            Ipv4Address dst = leaderBackhaulIfaces.GetAddress(j);
            OnOffHelper on("ns3::UdpSocketFactory", InetSocketAddress(dst, port + i*10 + j));
            on.SetConstantRate(DataRate("30kbps"));
            on.SetAttribute("PacketSize", UintegerValue(512));
            ApplicationContainer apps = on.Install(leaders.Get(i));
            apps.Start(Seconds(startTime + i*0.5));
            apps.Stop(Seconds(stopTime));
            PacketSinkHelper sink("ns3::UdpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), port + i*10 + j));
            apps = sink.Install(leaders.Get(j));
            apps.Start(Seconds(startTime));
            apps.Stop(Seconds(stopTime));
        }
    }
    NS_LOG_INFO("SetupInterClusterTraffic: tráfico entre líderes configurado");
}

// ------------------- main -------------------
int main(int argc, char *argv[])
{
    SimulationParams params;
    params.numClusters = 3;
    params.simulationTime = 60.0;
    params.leaderSpeed = 8.0; // speed for leaders (patrolling)
    params.interClusterDistance = 200.0;

    // Configure clusters (you can adjust)
    params.clusters.push_back({1, Vector(150,150,0), 4, 50.0, 3.0, "circular"});
    params.clusters.push_back({2, Vector(350,150,0), 3, 45.0, 2.5, "rectangular"});
    params.clusters.push_back({3, Vector(250,300,0), 3, 40.0, 2.0, "zigzag"});

    CommandLine cmd;
    cmd.AddValue("numClusters", "Número de clusters (>=2)", params.numClusters);
    cmd.AddValue("simTime", "Tiempo de simulación", params.simulationTime);
    cmd.AddValue("leaderSpeed", "Velocidad líderes", params.leaderSpeed);
    cmd.Parse(argc, argv);

    LogComponentEnable("ManetHierarchical_OpA", LOG_LEVEL_INFO);

    NS_LOG_INFO("MANET Jerárquica (Op A) - iniciando");
    std::string outDir = CreateOutputDirectory();

     // Create nodes
    NodeContainer leaders;
    std::vector<NodeContainer> clusterSubs(params.numClusters);
    NodeContainer allNodes;
    NodeContainer allSubordinates;

    leaders.Create(params.numClusters);
    for (uint32_t i=0;i<leaders.GetN();++i) allNodes.Add(leaders.Get(i));

    for (uint32_t i=0;i<params.numClusters;++i) {
        clusterSubs[i].Create(params.clusters[i].numNodes);
        for (uint32_t j=0;j<clusterSubs[i].GetN();++j) {
            allNodes.Add(clusterSubs[i].Get(j));
            allSubordinates.Add(clusterSubs[i].Get(j));
        }
    }

    NS_LOG_INFO("Total nodos: " << allNodes.GetN() << " (" << leaders.GetN() << " líderes + " << allSubordinates.GetN() << " subordinados)");

    // Mobility
    SetupLeaderMobility(leaders, params.clusters, params.leaderSpeed);
    for (uint32_t i=0;i<params.numClusters;++i) {
        SetupSubordinateMobility(clusterSubs[i], leaders.Get(i), params.clusters[i].mobilityRadius, params.clusters[i].subordinateSpeed);
        Simulator::Schedule(Seconds(0.5), &RepositionSubordinates, clusterSubs[i], leaders.Get(i), params.clusters[i].mobilityRadius);
    }

    // --- Physical WiFi setup ---
    WifiHelper wifi;
    wifi.SetStandard(WIFI_STANDARD_80211a);
    wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager",
                                 "DataMode", StringValue("OfdmRate6Mbps"),
                                 "RtsCtsThreshold", UintegerValue(0));
    YansWifiChannelHelper ch;
    ch.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
    ch.AddPropagationLoss("ns3::FriisPropagationLossModel");
    Ptr<YansWifiChannel> pch = ch.Create();
    YansWifiPhyHelper phy;
    phy.SetChannel(pch);
    WifiMacHelper mac;
    mac.SetType("ns3::AdhocWifiMac");

    // Install access WiFi on ALL nodes (device 0)
    NetDeviceContainer accessDevices = wifi.Install(phy, mac, allNodes);

    // Backhaul for leaders (separate channel/PHY) -> device index 1 on leaders
    YansWifiChannelHelper chBh;
    chBh.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
    chBh.AddPropagationLoss("ns3::FriisPropagationLossModel");
    Ptr<YansWifiChannel> pchBh = chBh.Create();
    YansWifiPhyHelper phyBh;
    phyBh.SetChannel(pchBh);
    WifiHelper wifiBh;
    wifiBh.SetStandard(WIFI_STANDARD_80211a);
    WifiMacHelper macBh;
    macBh.SetType("ns3::AdhocWifiMac");
    NetDeviceContainer leaderBhDevices = wifiBh.Install(phyBh, macBh, leaders);

    // Adjust tx power: access (lower) and backhaul (higher)
    for (uint32_t i=0;i<leaders.GetN();++i) {
        if (leaders.Get(i)->GetNDevices() > 0) {
            Ptr<NetDevice> d = leaders.Get(i)->GetDevice(0);
            Ptr<WifiNetDevice> w = DynamicCast<WifiNetDevice>(d);
            if (w) { Ptr<WifiPhy> p = w->GetPhy(); p->SetTxPowerStart(16.0); p->SetTxPowerEnd(16.0); }
        }
        if (leaders.Get(i)->GetNDevices() > 1) {
            Ptr<NetDevice> d = leaders.Get(i)->GetDevice(1);
            Ptr<WifiNetDevice> w = DynamicCast<WifiNetDevice>(d);
            if (w) { Ptr<WifiPhy> p = w->GetPhy(); p->SetTxPowerStart(23.0); p->SetTxPowerEnd(23.0); }
        }
    }

    // Build per-cluster NetDeviceContainers for IP assignment (access devices: device 0 for leader + device 0 for subs)
    std::vector<NetDeviceContainer> devPerCluster(params.numClusters);
    for (uint32_t i=0;i<params.numClusters;++i) {
        NetDeviceContainer c;
        c.Add(leaders.Get(i)->GetDevice(0)); // leader access device
        for (uint32_t j=0;j<clusterSubs[i].GetN();++j) c.Add(clusterSubs[i].Get(j)->GetDevice(0));
        devPerCluster[i] = c;
    }

    // --- Install Internet stacks BEFORE Assigning IPs ---
    // Subordinates: basic stack (no global OLSR)
    InternetStackHelper stackSubs;
    stackSubs.Install(allSubordinates);

    // Leaders: install OLSR (global)
    OlsrHelper olsr;
    Ipv4ListRoutingHelper list;
    list.Add(olsr, 10);
    InternetStackHelper stackLeaders;
    stackLeaders.SetRoutingHelper(list);
    stackLeaders.Install(leaders);

    // --- Assign IPs: separate subnets per cluster + backhaul subnet ---
    std::vector<Ipv4InterfaceContainer> ifacesPerCluster(params.numClusters);
    Ipv4AddressHelper ip;
    for (uint32_t i=0;i<params.numClusters;++i) {
        std::ostringstream net; net << "10." << (i+1) << ".0.0";
        ip.SetBase(net.str().c_str(), "255.255.255.0");
        ifacesPerCluster[i] = ip.Assign(devPerCluster[i]);
        ip.NewNetwork();
    }
    // Backhaul network among leaders
    Ipv4AddressHelper ipBh;
    ipBh.SetBase("192.168.100.0", "255.255.255.0");
    Ipv4InterfaceContainer leaderBhIfaces = ipBh.Assign(leaderBhDevices);

    // --- Static default routes in subordinates pointing to their leader IP (cluster-local) ---
    Ipv4StaticRoutingHelper staticHelper;
    for (uint32_t i=0;i<params.numClusters;++i) {
        Ipv4Address leaderClusterIp = ifacesPerCluster[i].GetAddress(0); // first device was leader
        for (uint32_t j=0;j<clusterSubs[i].GetN();++j) {
            Ptr<Node> sub = clusterSubs[i].Get(j);
            Ptr<Ipv4> ipv4 = sub->GetObject<Ipv4>();
            Ptr<NetDevice> dev = sub->GetDevice(0);
            int32_t iface = ipv4->GetInterfaceForDevice(dev);
            if (iface == -1) {
                NS_LOG_WARN("No interface found for subordinate " << sub->GetId());
                continue;
            }
            Ptr<Ipv4StaticRouting> sr = staticHelper.GetStaticRouting(ipv4);
            sr->SetDefaultRoute(leaderClusterIp, iface);
        }
    }

    // --- Set up inter-cluster traffic (leaders) ---
    SetupInterClusterTraffic(leaders, leaderBhIfaces, 5.0, params.simulationTime - 5.0);

    // --- Connectivity monitoring for leaders ---
    ScheduleConnectivityMonitoring(leaders, 1.0, params.simulationTime, 300.0);

    // --- NetAnim visualization ---
    AnimationInterface anim(outDir + "/manet-jerarquica-opA.xml");
    for (uint32_t i=0;i<leaders.GetN();++i) {
        anim.UpdateNodeDescription(leaders.Get(i), "Líder-" + std::to_string(i+1));
        anim.UpdateNodeColor(leaders.Get(i), 255, 0, 0);
        anim.UpdateNodeSize(leaders.Get(i)->GetId(), 8, 8);
    }
    uint8_t colors[][3] = {{0,0,255},{0,255,0},{255,128,0}};
    for (uint32_t i=0;i<params.numClusters;++i) {
        for (uint32_t j=0;j<clusterSubs[i].GetN();++j) {
            Ptr<Node> n = clusterSubs[i].Get(j);
            anim.UpdateNodeDescription(n, "C" + std::to_string(i+1) + "-N" + std::to_string(j+1));
            anim.UpdateNodeColor(n, colors[i%3][0], colors[i%3][1], colors[i%3][2]);
            anim.UpdateNodeSize(n->GetId(), 5, 5);
        }
    }
    anim.EnablePacketMetadata(true);
    anim.EnableIpv4RouteTracking(outDir + "/manet-jerarquica-opA-routes.xml", Seconds(0), Seconds(params.simulationTime), Seconds(1));
    // FlowMonitor
    FlowMonitorHelper flowmon;
    Ptr<FlowMonitor> monitor = flowmon.InstallAll();

    // Run simulation
    Simulator::Stop(Seconds(params.simulationTime));
    Simulator::Run();

    // Analysis
    monitor->CheckForLostPackets();
    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowmon.GetClassifier());
    std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats();

    uint32_t totalTx=0, totalRx=0;
    double totalThroughput=0.0;
    uint32_t interTx=0, interRx=0;
    double interThroughput=0.0;
    uint32_t interFlows=0;

    std::cout << "\n--- Flujos detectados ---\n";
    for (auto const &entry : stats) {
        if (entry.second.txPackets == 0) continue;
        Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(entry.first);
        double thr = entry.second.rxBytes * 8.0 / (params.simulationTime - 5.0) / 1000.0;
        double pdr = (entry.second.rxPackets * 100.0) / entry.second.txPackets;
        double delay = entry.second.rxPackets > 0 ? entry.second.delaySum.GetSeconds() / entry.second.rxPackets : 0.0;

        std::cout << "Flow " << entry.first << ": " << t.sourceAddress << " -> " << t.destinationAddress << "\n";
        std::cout << "  TX/RX: " << entry.second.txPackets << "/" << entry.second.rxPackets << "  PDR: " << pdr << "%  Throughput: " << thr << " kbps  Delay(ms): " << delay*1000.0 << "\n";

        totalTx += entry.second.txPackets;
        totalRx += entry.second.rxPackets;
        totalThroughput += thr;

        bool isInter = false;
        for (uint32_t i=0;i<leaders.GetN();++i) {
            Ipv4Address a = leaderBhIfaces.GetAddress(i);
            if (t.sourceAddress == a || t.destinationAddress == a) { isInter = true; break; }
        }
        if (isInter) { interTx += entry.second.txPackets; interRx += entry.second.rxPackets; interThroughput += thr; interFlows++; std::cout << "  [INTER-CLUSTER]\n"; }
    }

    std::cout << "\nResumen:\nTotal TX: " << totalTx << "  Total RX: " << totalRx << "  PDR geral: ";
    if (totalTx>0) std::cout << (totalRx*100.0/totalTx) << "%\n"; else std::cout << "N/A\n";
    std::cout << "Throughput total(kbps): " << totalThroughput << "\n";
    std::cout << "Líderes: mediciones totales: " << globalConnectivity.totalChecks << " conectadas: " << globalConnectivity.connectedChecks << " Ratio: " << (globalConnectivity.GetConnectivityRatio()*100.0) << "%\n";

    Simulator::Destroy();
    //NS_LOG_INFO("Simulación finalizada. Archivos: manet-jerarquica-opA.xml y manet-jerarquica-opA-routes.xml");
    NS_LOG_INFO("Simulación finalizada. Archivos en: " << outDir);
    return 0;
}
