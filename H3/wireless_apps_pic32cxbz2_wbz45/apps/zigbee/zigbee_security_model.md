---
grand_parent: Examples applications
parent: Zigbee
title: Zigbee Network Security Models
has_children: true
has_toc: false
nav_order: 235

family: PIC32CX-BZ
market:
  - wireless
---

# Zigbee Network Security Models

---
[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)
To satisfy a wide range of applications while maintaining low cost and power, Zigbee offers two network architectures and corresponding security models: **distributed** and **centralized**.

They basically differ in how they admit new devices into the network. The security model controls how a network key is distributed; and may control how network frame counters are initialized. The security model does not affect how messages are secured.

1. **Distributed security model :**
  * Provides a less-secured and simpler system
  * It has two devices types:
    * Routers
    * End devices
  * Here, a router can form a distributed security network, when it can’t ﬁnd any existing network.
  * Each router in the network can issue network keys. As more routers and devices join the network, the router through which new device joins the network will share the key.
  * To participate in distributed security networks, all router and end devices must be pre-conﬁgured with a link key that is used to encrypt the network key when passing it from a router parent to a newly joined node.
  * All the devices in the network encrypt messages with the same network key.


2. **Centralized security model :**
  * Provides higher security and is also more complicated.
  * It includes a third device type, the Trust Center (TC), which is usually also the network coordinator.
  * The Trust Center forms a centralized network, conﬁgures and authenticates routers and end devices to join a network.
  * The TC establishes a unique TC Link Key for each device on the network as they join and link keys for each pair of devices as requested.
  * The TC also determines the network key.
  * To participate in a centralized security network model, all entities must be pre-conﬁgured with a link key that is used to encrypt the network key when passing it from the TC to a newly joined entity. Only TC can share the network with the newly joining device.

All non coordinator ZigBee nodes shall be able to join a network supporting either model and adapt to the security conditions of the network they are joining.

<div style="text-align:center"><img src="doc/resources/SecurityModels.JPG" /></div>


### Key Exchange Process: Centralised Network

The diagram shows how a device is allowed to join Zigbee 3.0 network via the key exchange process in a centralized security model based network :


- The joiner device begins the join procedure by transmitting an unsecured beacon request frame (depending on whether the scan is an active or passive scan)

- It receives beacons from nearby routers and the NWK layer primitives parameter i.e. NetworkList will indicate all of the nearby PANs.

- The joiner device shall decide which PAN to join and shall issue an association request command to the router

- Upon receipt of an association request command, the router shall issue an association response command to be sent to the joiner.

- If the router is the Trust Center, it shall begin the authorization procedure by simply operating as a Trust Center. If the router is not the Trust Center, it shall begin the authorization procedure immediately by issuing an update device command request to the Trust Center.

- The Trust Center role in the authorization procedure shall be activated upon receipt of an incoming update-device command. It shall determine whether or not to allow the device onto the network. This decision will be based on its own security policies. If it decides to allow the device onto the network, it shall send the device the active network key.

- Once the network-level security is set-up , application-level security needs be setup for more secure communication, which is mandated in Zigbee 3.0 and will be used for all ongoing APS-layer communication.

<div style="text-align:center"><img src="doc/resources/centralized-network.JPG" /></div>


### Key Exchange Process: Distributed Network

The diagram shows how a device is allowed to join Zigbee 3.0 network in a distributed security model based network.

- The joiner device begins the join procedure by transmitting an unsecured beacon request frame (depending on whether the scan is an active or passive scan)

- It receives beacons from nearby routers and the NWK layer primitives parameter i.e. NetworkList will indicate all of the nearby PANs.

- The joiner device shall decide which PAN to join and shall issue an association request or rejoin request command to the router

- Upon receipt of an association request command, the router shall issue an association response command to be sent to the joiner.

- The router itself shall begin the authorization procedure. It shall determine whether or not to allow the device onto the network. This decision will be based on its own security policies. If it decides to allow the device onto the network, it shall send the device the active network key.

- Once the network-level security is set-up , application-level security needs be setup for more secure communication, which is mandated in Zigbee 3.0 and will be used for all ongoing APS-layer communication.

<div style="text-align:center"><img src="doc/resources/distributed-network.JPG"></div>



For more details regarding the network security models, please refer to, the Security section of the base device behaviour specification from Zigbee Alliance ->
<a href="https://zigbeealliance.org/developer_resources/zigbee-base-device-behavior-bdb-v1-0/" target="_top">Link to Base Device Behaviour Specification by Zigbee Alliance</a>


### Example Logs
Let us realize the concept of centralized network with the help of Combined Interface device (Zigbee Coordinator) and Thermostat(Zigbee router). Once the coordinator is powered-on, it forms a centralized network. The router which is powered on later, will join the already existing network. Now, the router is capable of supporting other new end devices to join the network via association process etc.


Shows the console output from a Combined Interface Application which is a Zigbee coordinator device type and a Thermostat application which is a Zigbee router device type.

<div style="text-align:center"><img src="doc/resources/centralized_network_combined-interface_thermostat.png" /></div>

The ZLO lights has the capability of forming its own network, though they are routers. If  Extended Lights(Zigbee router) was powered-on earlier to the coordinator, it wouldn't be able to find any network to join. In the absence, it will create a distributed network. Any end device/router shall be able to join the network via appropriate authentication process.  

Shows the console output from a Extended Light Application (Zigbee router) establishes a Distributed Network. A Color Scene controller (Zigbee end device) joins the network :
<div style="text-align:center"><img src="doc/resources/distributed-network_ext-lights_csc.png" /></div>

---
