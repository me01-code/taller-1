/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * MANET Simulation using OLSR Routing Protocol
 * This simulation demonstrates mobile ad-hoc networks with OLSR routing
 */

#include "ns3/olsr-module.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/wifi-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"
#include "ns3/flow-monitor-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ManetOlsrExample");

class ManetOlsrSimulation
{
public:
  ManetOlsrSimulation ();
  void Run (int nWifis, int nSinks, double totalTime, std::string rate,
            std::string phyMode, uint32_t nodeSpeed, uint32_t nodePause);

private:
  void CreateNodes ();
  void CreateDevices ();
  void InstallInternetStack ();
  void InstallApplications ();
  void SetupMobility ();
  
  uint32_t m_nWifis;
  uint32_t m_nSinks;
  double m_totalTime;
  std::string m_rate;
  std::string m_phyMode;
  uint32_t m_nodeSpeed;
  uint32_t m_nodePause;
  
  NodeContainer nodes;
  NetDeviceContainer devices;
  Ipv4InterfaceContainer interfaces;
};

ManetOlsrSimulation::ManetOlsrSimulation ()
{
}

void
ManetOlsrSimulation::CreateNodes ()
{
  NS_LOG_INFO ("Creating " << m_nWifis << " nodes.");
  nodes.Create (m_nWifis);
}

void
ManetOlsrSimulation::CreateDevices ()
{
  NS_LOG_INFO ("Creating devices and channels.");
  
  WifiHelper wifi;
  wifi.SetStandard (WIFI_STANDARD_80211b);
  
  YansWifiPhyHelper wifiPhy;
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  wifiPhy.SetChannel (wifiChannel.Create ());
  
  WifiMacHelper wifiMac;
  wifiMac.SetType ("ns3::AdhocWifiMac");
  
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                 "DataMode", StringValue (m_phyMode),
                                 "ControlMode", StringValue (m_phyMode));
  
  devices = wifi.Install (wifiPhy, wifiMac, nodes);
}

void
ManetOlsrSimulation::InstallInternetStack ()
{
  NS_LOG_INFO ("Installing Internet stack with OLSR routing.");
  
  OlsrHelper olsr;
  InternetStackHelper internet;
  internet.SetRoutingHelper (olsr);
  internet.Install (nodes);
  
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  interfaces = ipv4.Assign (devices);
}

void
ManetOlsrSimulation::SetupMobility ()
{
  NS_LOG_INFO ("Setting up mobility.");
  
  MobilityHelper mobility;
  ObjectFactory pos;
  pos.SetTypeId ("ns3::RandomRectanglePositionAllocator");
  pos.Set ("X", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=500.0]"));
  pos.Set ("Y", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=500.0]"));
  
  Ptr<PositionAllocator> taPositionAlloc = pos.Create ()->GetObject<PositionAllocator> ();
  
  std::stringstream ssSpeed;
  ssSpeed << "ns3::UniformRandomVariable[Min=0.0|Max=" << m_nodeSpeed << "]";
  std::stringstream ssPause;
  ssPause << "ns3::ConstantRandomVariable[Constant=" << m_nodePause << "]";
  
  mobility.SetMobilityModel ("ns3::RandomWaypointMobilityModel",
                             "Speed", StringValue (ssSpeed.str ()),
                             "Pause", StringValue (ssPause.str ()),
                             "PositionAllocator", PointerValue (taPositionAlloc));
  mobility.SetPositionAllocator (taPositionAlloc);
  mobility.Install (nodes);
}

void
ManetOlsrSimulation::InstallApplications ()
{
  NS_LOG_INFO ("Installing applications.");
  
  uint16_t port = 9;
  
  for (uint32_t i = 0; i < m_nSinks; i++)
    {
      PacketSinkHelper sink ("ns3::UdpSocketFactory",
                             InetSocketAddress (Ipv4Address::GetAny (), port));
      ApplicationContainer apps_sink = sink.Install (nodes.Get (i));
      apps_sink.Start (Seconds (0.0));
      apps_sink.Stop (Seconds (m_totalTime));
    }
  
  for (uint32_t i = m_nSinks; i < m_nWifis; i++)
    {
      OnOffHelper onoff ("ns3::UdpSocketFactory",
                         InetSocketAddress (interfaces.GetAddress (i % m_nSinks), port));
      onoff.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1.0]"));
      onoff.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0.0]"));
      onoff.SetAttribute ("DataRate", DataRateValue (DataRate (m_rate)));
      onoff.SetAttribute ("PacketSize", UintegerValue (1024));
      
      ApplicationContainer apps = onoff.Install (nodes.Get (i));
      apps.Start (Seconds (1.0));
      apps.Stop (Seconds (m_totalTime));
    }
}

void
ManetOlsrSimulation::Run (int nWifis, int nSinks, double totalTime, std::string rate,
                          std::string phyMode, uint32_t nodeSpeed, uint32_t nodePause)
{
  m_nWifis = nWifis;
  m_nSinks = nSinks;
  m_totalTime = totalTime;
  m_rate = rate;
  m_phyMode = phyMode;
  m_nodeSpeed = nodeSpeed;
  m_nodePause = nodePause;
  
  CreateNodes ();
  CreateDevices ();
  InstallInternetStack ();
  SetupMobility ();
  InstallApplications ();
  
  NS_LOG_INFO ("Run Simulation.");
  
  // Flow Monitor
  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor = flowmon.InstallAll ();
  
  Simulator::Stop (Seconds (m_totalTime));
  Simulator::Run ();
  
  // Print per flow statistics
  monitor->CheckForLostPackets ();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
  std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();
  
  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
    {
      Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
      std::cout << "Flow " << i->first << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";
      std::cout << "  Tx Packets: " << i->second.txPackets << "\n";
      std::cout << "  Tx Bytes:   " << i->second.txBytes << "\n";
      std::cout << "  Rx Packets: " << i->second.rxPackets << "\n";
      std::cout << "  Rx Bytes:   " << i->second.rxBytes << "\n";
      std::cout << "  Throughput: " << i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket.GetSeconds () - i->second.timeFirstTxPacket.GetSeconds ()) / 1024 / 1024 << " Mbps\n";
    }
  
  Simulator::Destroy ();
}

int
main (int argc, char *argv[])
{
  int nWifis = 10;
  int nSinks = 3;
  double totalTime = 100.0;
  std::string rate = "2048bps";
  std::string phyMode = "DsssRate11Mbps";
  uint32_t nodeSpeed = 10; // m/s
  uint32_t nodePause = 0;  // seconds
  
  CommandLine cmd;
  cmd.AddValue ("nWifis", "Number of wifi nodes", nWifis);
  cmd.AddValue ("nSinks", "Number of SINK traffic nodes", nSinks);
  cmd.AddValue ("totalTime", "Total simulation time (s)", totalTime);
  cmd.AddValue ("rate", "CBR traffic rate", rate);
  cmd.AddValue ("phyMode", "Wifi Phy mode", phyMode);
  cmd.AddValue ("nodeSpeed", "Node speed in RandomWaypoint model (m/s)", nodeSpeed);
  cmd.AddValue ("nodePause", "Node pause in RandomWaypoint model (s)", nodePause);
  cmd.Parse (argc, argv);
  
  ManetOlsrSimulation simulation;
  simulation.Run (nWifis, nSinks, totalTime, rate, phyMode, nodeSpeed, nodePause);
  
  return 0;
}
