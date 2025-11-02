/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2025 Taller Métodos Estocásticos
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Simulación de red MANET con protocolo OLSR
 * Autor: Taller Métodos Estocásticos
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/wifi-module.h"
#include "ns3/applications-module.h"
#include "ns3/olsr-helper.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/netanim-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("ManetOlsrSimulation");

/**
 * Clase para ejecutar simulación MANET con OLSR
 */
class ManetOlsrSimulation
{
public:
  ManetOlsrSimulation();
  void Run(int argc, char **argv);

private:
  void CreateNodes();
  void ConfigureWifi();
  void ConfigureMobility();
  void ConfigureRouting();
  void ConfigureApplications();
  void ConfigureTracing();
  void PrintResults();

  // Parámetros de simulación
  uint32_t m_nNodes;          // Número de nodos
  double m_simTime;           // Tiempo de simulación (segundos)
  double m_speed;             // Velocidad máxima de nodos (m/s)
  double m_pause;             // Tiempo de pausa (segundos)
  uint32_t m_packetSize;      // Tamaño de paquete (bytes)
  double m_interval;          // Intervalo entre paquetes (segundos)
  uint32_t m_port;            // Puerto UDP
  bool m_enableNetAnim;       // Habilitar NetAnim
  bool m_enablePcap;          // Habilitar trazas PCAP
  std::string m_phyMode;      // Modo PHY WiFi

  // Containers NS-3
  NodeContainer m_nodes;
  NetDeviceContainer m_devices;
  Ipv4InterfaceContainer m_interfaces;
  ApplicationContainer m_serverApps;
  ApplicationContainer m_clientApps;

  // Ayudantes
  Ptr<FlowMonitor> m_flowMonitor;
  FlowMonitorHelper m_flowHelper;
};

ManetOlsrSimulation::ManetOlsrSimulation()
    : m_nNodes(20),
      m_simTime(100.0),
      m_speed(10.0),
      m_pause(2.0),
      m_packetSize(512),
      m_interval(0.5),
      m_port(9),
      m_enableNetAnim(false),
      m_enablePcap(false),
      m_phyMode("DsssRate11Mbps")
{
}

void
ManetOlsrSimulation::Run(int argc, char **argv)
{
  // Configurar línea de comandos
  CommandLine cmd(__FILE__);
  cmd.AddValue("nodes", "Número de nodos", m_nNodes);
  cmd.AddValue("duration", "Duración de simulación (segundos)", m_simTime);
  cmd.AddValue("speed", "Velocidad máxima de nodos (m/s)", m_speed);
  cmd.AddValue("pause", "Tiempo de pausa (segundos)", m_pause);
  cmd.AddValue("packetSize", "Tamaño de paquete (bytes)", m_packetSize);
  cmd.AddValue("interval", "Intervalo entre paquetes (segundos)", m_interval);
  cmd.AddValue("netanim", "Habilitar NetAnim (0/1)", m_enableNetAnim);
  cmd.AddValue("pcap", "Habilitar trazas PCAP (0/1)", m_enablePcap);
  cmd.Parse(argc, argv);

  // Validar parámetros
  NS_ASSERT(m_nNodes >= 2);
  NS_ASSERT(m_simTime > 0);

  NS_LOG_INFO("Configurando simulación MANET OLSR");
  NS_LOG_INFO("Nodos: " << m_nNodes);
  NS_LOG_INFO("Duración: " << m_simTime << " segundos");
  NS_LOG_INFO("Velocidad máxima: " << m_speed << " m/s");

  // Configurar tiempo de simulación
  Time::SetResolution(Time::NS);

  // Crear y configurar red
  CreateNodes();
  ConfigureWifi();
  ConfigureMobility();
  ConfigureRouting();
  ConfigureApplications();
  ConfigureTracing();

  // Ejecutar simulación
  NS_LOG_INFO("Iniciando simulación...");
  Simulator::Stop(Seconds(m_simTime));
  Simulator::Run();

  // Analizar resultados
  PrintResults();

  // Limpiar
  Simulator::Destroy();
}

void
ManetOlsrSimulation::CreateNodes()
{
  NS_LOG_INFO("Creando " << m_nNodes << " nodos");
  m_nodes.Create(m_nNodes);
}

void
ManetOlsrSimulation::ConfigureWifi()
{
  NS_LOG_INFO("Configurando WiFi ad-hoc");

  // Configurar PHY
  WifiHelper wifi;
  wifi.SetStandard(WIFI_STANDARD_80211b);

  YansWifiPhyHelper wifiPhy;
  YansWifiChannelHelper wifiChannel;
  wifiChannel.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
  wifiChannel.AddPropagationLoss("ns3::RangePropagationLossModel",
                                  "MaxRange", DoubleValue(250.0));
  wifiPhy.SetChannel(wifiChannel.Create());

  // Configurar MAC
  WifiMacHelper wifiMac;
  wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager",
                                "DataMode", StringValue(m_phyMode),
                                "ControlMode", StringValue(m_phyMode));

  // Modo ad-hoc
  wifiMac.SetType("ns3::AdhocWifiMac");

  // Crear dispositivos
  m_devices = wifi.Install(wifiPhy, wifiMac, m_nodes);
}

void
ManetOlsrSimulation::ConfigureMobility()
{
  NS_LOG_INFO("Configurando movilidad");

  MobilityHelper mobility;

  // Definir área de simulación
  mobility.SetPositionAllocator("ns3::RandomRectanglePositionAllocator",
                                "X", StringValue("ns3::UniformRandomVariable[Min=0.0|Max=1000.0]"),
                                "Y", StringValue("ns3::UniformRandomVariable[Min=0.0|Max=1000.0]"));

  // Modelo Random Waypoint
  std::stringstream ssSpeed;
  ssSpeed << "ns3::UniformRandomVariable[Min=0.0|Max=" << m_speed << "]";

  std::stringstream ssPause;
  ssPause << "ns3::ConstantRandomVariable[Constant=" << m_pause << "]";

  mobility.SetMobilityModel("ns3::RandomWaypointMobilityModel",
                            "Speed", StringValue(ssSpeed.str()),
                            "Pause", StringValue(ssPause.str()),
                            "PositionAllocator", PointerValue(
                              CreateObject<RandomRectanglePositionAllocator>(
                                CreateObjectWithAttributes<UniformRandomVariable>(
                                  "Min", DoubleValue(0.0), "Max", DoubleValue(1000.0)),
                                CreateObjectWithAttributes<UniformRandomVariable>(
                                  "Min", DoubleValue(0.0), "Max", DoubleValue(1000.0)))));

  mobility.Install(m_nodes);
}

void
ManetOlsrSimulation::ConfigureRouting()
{
  NS_LOG_INFO("Configurando protocolo OLSR");

  // Configurar stack Internet con OLSR
  OlsrHelper olsr;
  InternetStackHelper internet;
  internet.SetRoutingHelper(olsr);
  internet.Install(m_nodes);

  // Asignar direcciones IP
  Ipv4AddressHelper address;
  address.SetBase("10.1.1.0", "255.255.255.0");
  m_interfaces = address.Assign(m_devices);
}

void
ManetOlsrSimulation::ConfigureApplications()
{
  NS_LOG_INFO("Configurando aplicaciones");

  // Configurar servidor UDP en primer nodo
  UdpServerHelper server(m_port);
  m_serverApps = server.Install(m_nodes.Get(0));
  m_serverApps.Start(Seconds(1.0));
  m_serverApps.Stop(Seconds(m_simTime));

  // Configurar clientes UDP en otros nodos
  for (uint32_t i = 1; i < m_nNodes; ++i)
    {
      UdpClientHelper client(m_interfaces.GetAddress(0), m_port);
      client.SetAttribute("MaxPackets", UintegerValue(100000));
      client.SetAttribute("Interval", TimeValue(Seconds(m_interval)));
      client.SetAttribute("PacketSize", UintegerValue(m_packetSize));

      ApplicationContainer clientApp = client.Install(m_nodes.Get(i));
      clientApp.Start(Seconds(2.0 + i * 0.1));
      clientApp.Stop(Seconds(m_simTime));
      m_clientApps.Add(clientApp);
    }
}

void
ManetOlsrSimulation::ConfigureTracing()
{
  NS_LOG_INFO("Configurando trazas");

  // PCAP
  if (m_enablePcap)
    {
      YansWifiPhyHelper wifiPhy;
      wifiPhy.EnablePcapAll("manet-olsr");
    }

  // NetAnim
  if (m_enableNetAnim)
    {
      AnimationInterface anim("manet-olsr-animation.xml");
      anim.EnablePacketMetadata(true);

      // Personalizar visualización de nodos
      for (uint32_t i = 0; i < m_nNodes; ++i)
        {
          anim.UpdateNodeDescription(m_nodes.Get(i), "Node" + std::to_string(i));
          anim.UpdateNodeColor(m_nodes.Get(i), 0, 255, 0);
          if (i == 0)
            {
              anim.UpdateNodeColor(m_nodes.Get(0), 255, 0, 0); // Servidor en rojo
            }
        }
    }

  // FlowMonitor
  m_flowMonitor = m_flowHelper.InstallAll();
}

void
ManetOlsrSimulation::PrintResults()
{
  NS_LOG_INFO("Analizando resultados");

  // Obtener estadísticas del FlowMonitor
  m_flowMonitor->CheckForLostPackets();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(m_flowHelper.GetClassifier());
  std::map<FlowId, FlowMonitor::FlowStats> stats = m_flowMonitor->GetFlowStats();

  // Variables para métricas
  double totalTxPackets = 0;
  double totalRxPackets = 0;
  double totalTxBytes = 0;
  double totalRxBytes = 0;
  double totalDelay = 0;
  uint32_t flowCount = 0;

  std::cout << "\n=== Resultados de la Simulación MANET OLSR ===" << std::endl;
  std::cout << "\nEstadísticas por flujo:" << std::endl;

  for (auto const &flow : stats)
    {
      Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(flow.first);

      std::cout << "\nFlujo " << flow.first << " (" << t.sourceAddress << " -> "
                << t.destinationAddress << ")" << std::endl;
      std::cout << "  Tx Packets: " << flow.second.txPackets << std::endl;
      std::cout << "  Rx Packets: " << flow.second.rxPackets << std::endl;
      std::cout << "  Lost Packets: " << flow.second.lostPackets << std::endl;

      if (flow.second.rxPackets > 0)
        {
          double pdr = (double)flow.second.rxPackets / (double)flow.second.txPackets * 100.0;
          double avgDelay = flow.second.delaySum.GetSeconds() / flow.second.rxPackets;

          std::cout << "  PDR: " << pdr << " %" << std::endl;
          std::cout << "  Avg Delay: " << avgDelay * 1000 << " ms" << std::endl;
          std::cout << "  Throughput: " << flow.second.rxBytes * 8.0 / m_simTime / 1024
                    << " kbps" << std::endl;

          totalDelay += avgDelay * flow.second.rxPackets;
        }

      totalTxPackets += flow.second.txPackets;
      totalRxPackets += flow.second.rxPackets;
      totalTxBytes += flow.second.txBytes;
      totalRxBytes += flow.second.rxBytes;
      flowCount++;
    }

  // Métricas globales
  std::cout << "\n=== Métricas Globales ===" << std::endl;
  std::cout << "Número de flujos: " << flowCount << std::endl;
  std::cout << "Total Tx Packets: " << totalTxPackets << std::endl;
  std::cout << "Total Rx Packets: " << totalRxPackets << std::endl;
  std::cout << "Total Lost Packets: " << (totalTxPackets - totalRxPackets) << std::endl;

  if (totalTxPackets > 0)
    {
      double globalPDR = totalRxPackets / totalTxPackets * 100.0;
      std::cout << "Global PDR: " << globalPDR << " %" << std::endl;
    }

  if (totalRxPackets > 0)
    {
      double avgDelay = totalDelay / totalRxPackets;
      std::cout << "Average Delay: " << avgDelay * 1000 << " ms" << std::endl;
    }

  if (m_simTime > 0)
    {
      double throughput = totalRxBytes * 8.0 / m_simTime / 1024;
      std::cout << "Network Throughput: " << throughput << " kbps" << std::endl;
    }

  std::cout << "\n=== Fin de Resultados ===" << std::endl;
}

int
main(int argc, char *argv[])
{
  ManetOlsrSimulation simulation;
  simulation.Run(argc, argv);
  return 0;
}
