/*******************************************************************************
  ZCL ZLL Scenes cluster Header File

  Company:
    Microchip Technology Inc.

  File Name:
    zclZllScenesCluster.h

  Summary:
    The header file describes the ZCL Scenes Cluster and its interface.

  Description:
    The file describes the types and interface of the ZCL Scenes Cluster.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END


#ifndef _ZCLSCENESCLUSTER_H
#define _ZCLSCENESCLUSTER_H

/*!
Attributes and commands for determining basic information about a device,
setting user device information such as location, enabling a device and resetting it
to factory defaults.
*/

/*******************************************************************************
                   Includes section
*******************************************************************************/

#include <zcl/include/zcl.h>
#include <zcl/include/clusters.h>
#include <zcl/include/zclZllOnOffCluster.h>
#include <zcl/include/zclZllLevelControlCluster.h>
#include <zcl/include/zclZllColorControlCluster.h>

/*******************************************************************************
                   Define(s) section
*******************************************************************************/

/**
 * \brief Scenes Cluster scene name support bit
*/

#define ZCL_SCENES_CLUSTER_NAME_SUPPORT_FLAG  0x80

#define ZCL_SCENES_CLUSTER_COPY_ALL_SCENES 0x01

/**
 * \brief Scenes Cluster server attributes amount
*/

#define ZCL_SCENES_CLUSTER_SERVER_ATTRIBUTES_AMOUNT ${SC_SERVERATTRIBUTECOUNT}

/**
 * \brief Scenes Cluster client attributes amount. Clinet doesn't have attributes.
*/

#define ZCL_SCENES_CLUSTER_CLIENT_ATTRIBTUES_AMOUNT ${SCC_CLIENTATTRIBUTECOUNT}

/**
 * \brief Scenes Cluster commands amount
*/

#define ZCL_SCENES_SERVER_CLUSTER_COMMANDS_AMOUNT ${SC_SERVERCOMMANDCOUNT}
#define ZCL_SCENES_CLIENT_CLUSTER_COMMANDS_AMOUNT ${SCC_CLIENTCOMMANDCOUNT}

/**
 * \brief Scenes ClusterVersion attribute
*/
#define ZCL_SCENES_CLUSTER_VERSION_ATTRIBUTE_ID              CCPU_TO_LE16(0xfffd)

/**
 * \brief Scenes ClusterVersion attribute identifier
*/

#define ZCL_SCENES_CLUSTER_SCENE_COUNT_SERVER_ATTRIBUTE_ID    CCPU_TO_LE16(0x0000)
#define ZCL_SCENES_CLUSTER_CURRENT_SCENE_SERVER_ATTRIBUTE_ID  CCPU_TO_LE16(0x0001)
#define ZCL_SCENES_CLUSTER_CURRENT_GROUP_SERVER_ATTRIBUTE_ID  CCPU_TO_LE16(0x0002)
#define ZCL_SCENES_CLUSTER_SCENE_VALID_SERVER_ATTRIBUTE_ID    CCPU_TO_LE16(0x0003)
#define ZCL_SCENES_CLUSTER_NAME_SUPPORT_SERVER_ATTRIBUTE_ID   CCPU_TO_LE16(0x0004)

/**
 * \brief Scenes Cluster client's command identifiers
*/

#define ZCL_SCENES_CLUSTER_ADD_SCENE_COMMAND_ID            0x00
#define ZCL_SCENES_CLUSTER_VIEW_SCENE_COMMAND_ID           0x01
#define ZCL_SCENES_CLUSTER_REMOVE_SCENE_COMMAND_ID         0x02
#define ZCL_SCENES_CLUSTER_REMOVE_ALL_SCENES_COMMAND_ID    0x03
#define ZCL_SCENES_CLUSTER_STORE_SCENE_COMMAND_ID          0x04
#define ZCL_SCENES_CLUSTER_RECALL_SCENE_COMMAND_ID         0x05
#define ZCL_SCENES_CLUSTER_GET_SCENE_MEMBERSHIP_COMMAND_ID 0x06
#define ZCL_SCENES_CLUSTER_ENHANCED_ADD_SCENE_COMMAND_ID   0x40
#define ZCL_SCENES_CLUSTER_ENHANCED_VIEW_SCENE_COMMAND_ID  0x41
#define ZCL_SCENES_CLUSTER_COPY_SCENE_COMMAND_ID           0x42

/**
 * \brief Scenes Cluster servers's command identifiers
*/

#define ZCL_SCENES_CLUSTER_ADD_SCENE_RESPONSE_COMMAND_ID         0x00
#define ZCL_SCENES_CLUSTER_VIEW_SCENE_RESPONSE_COMMAND_ID        0x01
#define ZCL_SCENES_CLUSTER_REMOVE_SCENE_RESPONSE_COMMAND_ID      0x02
#define ZCL_SCENES_CLUSTER_REMOVE_ALL_SCENES_RESPONSE_COMMAND_ID 0x03
#define ZCL_SCENES_CLUSTER_STORE_SCENE_RESPONSE_COMMAND_ID       0x04
#define ZCL_SCENES_CLUSTER_GET_SCENE_MEMBERSHIP_RESPONSE_COMMAND_ID 0x06
#define ZCL_SCENES_CLUSTER_ENHANCED_ADD_SCENE_RESPONSE_COMMAND_ID 0x40
#define ZCL_SCENES_CLUSTER_ENHANCED_VIEW_SCENE_RESPONSE_COMMAND_ID 0x41
#define ZCL_SCENES_CLUSTER_COPY_SCENE_RESPONSE_COMMAND_ID        0x42

/**
 * \brief Scenes Cluster server define attributes macros
*/

#define ZCL_DEFINE_SCENES_CLUSTER_SERVER_ATTRIBUTES() \
<#if (SCENES_CLUSTER_CS != "CLIENT")  && (SC_SCENECOUNT == true) >
    DEFINE_ATTRIBUTE(sceneCount, ZCL_READONLY_ATTRIBUTE, ZCL_SCENES_CLUSTER_SCENE_COUNT_SERVER_ATTRIBUTE_ID, ZCL_U8BIT_DATA_TYPE_ID), \
</#if>
<#if (SCENES_CLUSTER_CS != "CLIENT")  && (SC_CURRENTSCENE == true) >
    DEFINE_ATTRIBUTE(currentScene, ZCL_READONLY_ATTRIBUTE, ZCL_SCENES_CLUSTER_CURRENT_SCENE_SERVER_ATTRIBUTE_ID, ZCL_U8BIT_DATA_TYPE_ID), \
</#if>
<#if (SCENES_CLUSTER_CS != "CLIENT")  && (SC_CURRENTGROUP == true) >
    DEFINE_ATTRIBUTE_WITH_BOUNDARY(currentGroup, ZCL_READONLY_ATTRIBUTE, ZCL_SCENES_CLUSTER_CURRENT_GROUP_SERVER_ATTRIBUTE_ID, ZCL_U16BIT_DATA_TYPE_ID, 0x0000, 0xFFF7), \
</#if>
<#if (SCENES_CLUSTER_CS != "CLIENT")  && (SC_SCENEVALID == true) >
    DEFINE_ATTRIBUTE(sceneValid, ZCL_READONLY_ATTRIBUTE, ZCL_SCENES_CLUSTER_SCENE_VALID_SERVER_ATTRIBUTE_ID, ZCL_BOOLEAN_DATA_TYPE_ID), \
</#if>
<#if (SCENES_CLUSTER_CS != "CLIENT")  && (SC_SSNAMESUPPORT == true) >
    DEFINE_ATTRIBUTE(nameSupport, ZCL_READONLY_ATTRIBUTE, ZCL_SCENES_CLUSTER_NAME_SUPPORT_SERVER_ATTRIBUTE_ID, ZCL_8BIT_BITMAP_DATA_TYPE_ID), \
</#if>
<#if (SCENES_CLUSTER_CS != "CLIENT")  && (SCS_CLUSTERREVISION == true) >
    DEFINE_ATTRIBUTE(clusterVersion, ZCL_READONLY_ATTRIBUTE, ZCL_SCENES_CLUSTER_VERSION_ATTRIBUTE_ID, ZCL_U16BIT_DATA_TYPE_ID)
</#if>

/**
 * \brief Scenes Cluster client define attributes macros
*/

#define ZCL_DEFINE_SCENES_CLUSTER_CLIENT_ATTRIBUTES() \
<#if (SCENES_CLUSTER_CS != "SERVER")  && (SCC_CLUSTERREVISION == true) >
    DEFINE_ATTRIBUTE(clusterVersion, ZCL_READONLY_ATTRIBUTE, ZCL_SCENES_CLUSTER_VERSION_ATTRIBUTE_ID, ZCL_U16BIT_DATA_TYPE_ID)
</#if>

/**
 * \brief On/Off Cluster define commands macros
*/

#define ZCL_DEFINE_SCENES_CLUSTER_SERVER_COMMANDS(addSceneCommandInd, viewSceneCommandInd, removeSceneCommandInd, \
          removeAllScenesCommandInd, storeSceneCommandInd, recallSceneCommandInd, \
          getSceneMembershipCommandInd, enhancedAddSceneCommandInd, enhancedViewSceneCommandInd, copySceneCommandInd) \
<#if (SCENES_CLUSTER_CS != "CLIENT")  && (SC_ADDSCENE == true) >
    DEFINE_COMMAND(addSceneCommand, ZCL_SCENES_CLUSTER_ADD_SCENE_COMMAND_ID,  \
                     COMMAND_OPTIONS(CLIENT_TO_SERVER, ZCL_THERE_IS_RELEVANT_RESPONSE, ZCL_COMMAND_ACK), \
                     addSceneCommandInd), \
</#if>
<#if (SCENES_CLUSTER_CS != "CLIENT")  && (SC_VIEWSCENE == true) >
    DEFINE_COMMAND(viewSceneCommand, ZCL_SCENES_CLUSTER_VIEW_SCENE_COMMAND_ID, \
                     COMMAND_OPTIONS(CLIENT_TO_SERVER, ZCL_THERE_IS_RELEVANT_RESPONSE, ZCL_COMMAND_ACK), \
                     viewSceneCommandInd), \
</#if>
<#if (SCENES_CLUSTER_CS != "CLIENT")  && (SC_REMOVESCENE == true) >
    DEFINE_COMMAND(removeSceneCommand, ZCL_SCENES_CLUSTER_REMOVE_SCENE_COMMAND_ID, \
                      COMMAND_OPTIONS(CLIENT_TO_SERVER, ZCL_THERE_IS_RELEVANT_RESPONSE, ZCL_COMMAND_ACK), \
                     removeSceneCommandInd), \
</#if>
<#if (SCENES_CLUSTER_CS != "CLIENT")  && (SC_REMOVESCENEALL == true) >
    DEFINE_COMMAND(removeAllScenesCommand, ZCL_SCENES_CLUSTER_REMOVE_ALL_SCENES_COMMAND_ID,  \
                     COMMAND_OPTIONS(CLIENT_TO_SERVER, ZCL_THERE_IS_RELEVANT_RESPONSE, ZCL_COMMAND_ACK), \
                     removeAllScenesCommandInd), \
</#if>
<#if (SCENES_CLUSTER_CS != "CLIENT")  && (SC_STORESCENE == true) >
    DEFINE_COMMAND(storeSceneCommand, ZCL_SCENES_CLUSTER_STORE_SCENE_COMMAND_ID, \
                     COMMAND_OPTIONS(CLIENT_TO_SERVER, ZCL_THERE_IS_RELEVANT_RESPONSE, ZCL_COMMAND_ACK), \
                     storeSceneCommandInd), \
</#if>
<#if (SCENES_CLUSTER_CS != "CLIENT")  && (SC_RECALLSCENE == true) >
    DEFINE_COMMAND(recallSceneCommand, ZCL_SCENES_CLUSTER_RECALL_SCENE_COMMAND_ID, \
                     COMMAND_OPTIONS(CLIENT_TO_SERVER, ZCL_THERE_IS_NO_RELEVANT_RESPONSE, ZCL_COMMAND_ACK), \
                     recallSceneCommandInd), \
</#if>
<#if (SCENES_CLUSTER_CS != "CLIENT")  && (SC_SCENEMEM == true) >
    DEFINE_COMMAND(getSceneMembershipCommand, ZCL_SCENES_CLUSTER_GET_SCENE_MEMBERSHIP_COMMAND_ID, \
                     COMMAND_OPTIONS(CLIENT_TO_SERVER, ZCL_THERE_IS_RELEVANT_RESPONSE, ZCL_COMMAND_ACK), \
                     getSceneMembershipCommandInd), \
</#if>
<#if (SCENES_CLUSTER_CS != "CLIENT")  && (SC_ENHANCEDADDSCENE == true) >
    DEFINE_COMMAND(enhancedAddSceneCommand, ZCL_SCENES_CLUSTER_ENHANCED_ADD_SCENE_COMMAND_ID, \
                     COMMAND_OPTIONS(CLIENT_TO_SERVER, ZCL_THERE_IS_RELEVANT_RESPONSE, ZCL_COMMAND_ACK), \
                     enhancedAddSceneCommandInd), \
</#if>
<#if (SCENES_CLUSTER_CS != "CLIENT")  && (SC_ENHANCEDVIEWSCENE == true) >
    DEFINE_COMMAND(enhancedViewSceneCommand, ZCL_SCENES_CLUSTER_ENHANCED_VIEW_SCENE_COMMAND_ID, \
                     COMMAND_OPTIONS(CLIENT_TO_SERVER, ZCL_THERE_IS_RELEVANT_RESPONSE, ZCL_COMMAND_ACK), \
                     enhancedViewSceneCommandInd), \
</#if>
<#if (SCENES_CLUSTER_CS != "CLIENT")  && (SC_COPYSCENE == true) >
    DEFINE_COMMAND(copySceneCommand, ZCL_SCENES_CLUSTER_COPY_SCENE_COMMAND_ID, \
                     COMMAND_OPTIONS(CLIENT_TO_SERVER, ZCL_THERE_IS_RELEVANT_RESPONSE, ZCL_COMMAND_ACK), \
                     copySceneCommandInd)
</#if>

#define ZCL_DEFINE_SCENES_CLUSTER_CLIENT_COMMANDS(addSceneResponseCommandInd, viewSceneResponseCommandInd, \
          removeSceneResponseCommandInd, removeAllScenesResponseCommandInd, storeSceneResponseCommandInd, \
          getSceneMembershipResponseCommandInd, enhancedAddSceneResponseCommandInd, enhancedViewSceneResponseCommandInd, \
          copySceneResponseCommandInd) \
<#if (SCENES_CLUSTER_CS != "SERVER")  && (SCC_ADDSCENERESP == true) >
    DEFINE_COMMAND(addSceneResponseCommand, ZCL_SCENES_CLUSTER_ADD_SCENE_RESPONSE_COMMAND_ID, \
                      COMMAND_OPTIONS(SERVER_TO_CLIENT, ZCL_THERE_IS_NO_RELEVANT_RESPONSE, ZCL_COMMAND_ACK), \
                     addSceneResponseCommandInd), \
</#if>
<#if (SCENES_CLUSTER_CS != "SERVER")  && (SCC_VIEWSCENERES == true) >
    DEFINE_COMMAND(viewSceneResponseCommand, ZCL_SCENES_CLUSTER_VIEW_SCENE_RESPONSE_COMMAND_ID,  \
                     COMMAND_OPTIONS(SERVER_TO_CLIENT, ZCL_THERE_IS_NO_RELEVANT_RESPONSE, ZCL_COMMAND_ACK), \
                     viewSceneResponseCommandInd), \
</#if>
<#if (SCENES_CLUSTER_CS != "SERVER")  && (SCC_REMOVESCENERES == true) >
    DEFINE_COMMAND(removeSceneResponseCommand, ZCL_SCENES_CLUSTER_REMOVE_SCENE_RESPONSE_COMMAND_ID, \
                     COMMAND_OPTIONS(SERVER_TO_CLIENT, ZCL_THERE_IS_NO_RELEVANT_RESPONSE, ZCL_COMMAND_ACK), \
                     removeSceneResponseCommandInd), \
</#if>
<#if (SCENES_CLUSTER_CS != "SERVER")  && (SCC_REMOVESCENEALLRES == true) >
    DEFINE_COMMAND(removeAllScenesResponseCommand, ZCL_SCENES_CLUSTER_REMOVE_ALL_SCENES_RESPONSE_COMMAND_ID, \
                      COMMAND_OPTIONS(SERVER_TO_CLIENT, ZCL_THERE_IS_NO_RELEVANT_RESPONSE, ZCL_COMMAND_ACK), \
                     removeAllScenesResponseCommandInd), \
</#if>
<#if (SCENES_CLUSTER_CS != "SERVER")  && (SCC_STORESCENERES == true) >
    DEFINE_COMMAND(storeSceneResponseCommand, ZCL_SCENES_CLUSTER_STORE_SCENE_RESPONSE_COMMAND_ID,  \
                     COMMAND_OPTIONS(SERVER_TO_CLIENT, ZCL_THERE_IS_NO_RELEVANT_RESPONSE, ZCL_COMMAND_ACK), \
                     storeSceneResponseCommandInd), \
</#if>
<#if (SCENES_CLUSTER_CS != "SERVER")  && (SCC_SCENEMEMRES == true) >
    DEFINE_COMMAND(getSceneMembershipResponseCommand, ZCL_SCENES_CLUSTER_GET_SCENE_MEMBERSHIP_RESPONSE_COMMAND_ID, \
                     COMMAND_OPTIONS(SERVER_TO_CLIENT, ZCL_THERE_IS_NO_RELEVANT_RESPONSE, ZCL_COMMAND_ACK), \
                     getSceneMembershipResponseCommandInd), \
</#if>
<#if (SCENES_CLUSTER_CS != "SERVER")  && (SCC_ENHANCEDADDSCENERES == true) >
    DEFINE_COMMAND(enhancedAddSceneResponseCommand, ZCL_SCENES_CLUSTER_ENHANCED_ADD_SCENE_RESPONSE_COMMAND_ID, \
                     COMMAND_OPTIONS(SERVER_TO_CLIENT, ZCL_THERE_IS_NO_RELEVANT_RESPONSE, ZCL_COMMAND_ACK), \
                     enhancedAddSceneResponseCommandInd), \
</#if>
<#if (SCENES_CLUSTER_CS != "SERVER")  && (SCC_ENHANCEDVIEWSCENERES == true) >
    DEFINE_COMMAND(enhancedViewSceneResponseCommand, ZCL_SCENES_CLUSTER_ENHANCED_VIEW_SCENE_RESPONSE_COMMAND_ID, \
                     COMMAND_OPTIONS(SERVER_TO_CLIENT, ZCL_THERE_IS_NO_RELEVANT_RESPONSE, ZCL_COMMAND_ACK), \
                     enhancedViewSceneResponseCommandInd), \
</#if>
<#if (SCENES_CLUSTER_CS != "SERVER")  && (SCC_COPYSCENERES == true) >
    DEFINE_COMMAND(copySceneResponseCommand, ZCL_SCENES_CLUSTER_COPY_SCENE_RESPONSE_COMMAND_ID, \
                     COMMAND_OPTIONS(SERVER_TO_CLIENT, ZCL_THERE_IS_NO_RELEVANT_RESPONSE, ZCL_COMMAND_ACK), \
                     copySceneResponseCommandInd)
</#if>

/**
 * \brief Scene Cluster definition macros
*/

#define SCENES_CLUSTER_ZCL_CLIENT_CLUSTER_TYPE(clattributes, clcommands) \
{ \
   .id = SCENES_CLUSTER_ID, \
   .options = { \
                .type = ZCL_CLIENT_CLUSTER_TYPE, \
                .security = ZCL_NETWORK_KEY_CLUSTER_SECURITY, \
              }, \
   .attributesAmount = ZCL_SCENES_CLUSTER_CLIENT_ATTRIBTUES_AMOUNT, \
   .attributes = (uint8_t *)clattributes, \
   .commandsAmount = ZCL_SCENES_CLIENT_CLUSTER_COMMANDS_AMOUNT, \
   .commands = (uint8_t *)clcommands \
}

#define SCENES_CLUSTER_ZCL_SERVER_CLUSTER_TYPE(clattributes, clcommands) \
{ \
   .id = SCENES_CLUSTER_ID, \
   .options = { \
                .type = ZCL_SERVER_CLUSTER_TYPE, \
                .security = ZCL_NETWORK_KEY_CLUSTER_SECURITY, \
              }, \
   .attributesAmount = ZCL_SCENES_CLUSTER_SERVER_ATTRIBUTES_AMOUNT, \
   .attributes = (uint8_t *)clattributes, \
   .commandsAmount = ZCL_SCENES_SERVER_CLUSTER_COMMANDS_AMOUNT, \
   .commands = (uint8_t *)clcommands \
}

#define DEFINE_SCENES_CLUSTER(cltype, clattributes, clcommands) \
    SCENES_CLUSTER_##cltype(clattributes, clcommands)

/******************************************************************************
                    Types section
******************************************************************************/
BEGIN_PACK

/**
 * \brief Add Scene Command Payload format.
 */

typedef struct PACK
{
  uint16_t groupId;
  uint8_t sceneId;
  uint16_t transitionTime;
  uint8_t name[1];
} ZCL_AddScene_t;

/**
 * \brief View Scene Command Payload format.
 */

typedef struct PACK
{
  uint16_t groupId;
  uint8_t sceneId;
} ZCL_ViewScene_t;

/**
 * \brief Remove Scene Command Payload format.
 */

typedef struct PACK
{
  uint16_t groupId;
  uint8_t sceneId;
} ZCL_RemoveScene_t;

/**
 * \brief Remove All Scenes Command Payload format.
 */

typedef struct PACK
{
  uint16_t groupId;
} ZCL_RemoveAllScenes_t;

/**
 * \brief Store Scene Command Payload format.
 */

typedef struct PACK
{
  uint16_t groupId;
  uint8_t sceneId;
} ZCL_StoreScene_t;

/**
 * \brief Recall Scene Command Payload format.
 */

typedef struct PACK
{
  uint16_t groupId;
  uint8_t sceneId;
  uint16_t transitionTime;
} ZCL_RecallScene_t;

/**
 * \brief Get Scene Membership Command Payload format.
 */

typedef struct PACK
{
  uint16_t groupId;
} ZCL_GetSceneMembership_t;

/**
 * \brief Enhanced Add Scene Command Payload format.
 */

typedef struct PACK
{
  uint16_t groupId;
  uint8_t sceneId;
  uint16_t transitionTime;
  uint8_t name[1];
  ZCL_OnOffClusterExtensionFieldSet_t onOffClusterExtFields;
  ZCL_LevelControlClusterExtensionFieldSet_t levelControlClusterExtFields;
  ZCL_ColorControlClusterExtensionFieldSet_t colorControlClusterExtFields;
} ZCL_EnhancedAddScene_t;

/**
 * \brief Enhanced View Scene Command Payload format.
 */

typedef struct PACK
{
  uint16_t groupId;
  uint8_t sceneId;
} ZCL_EnhancedViewScene_t;

/**
 * \brief Copy Scene Command Payload format.
 */

typedef struct PACK
{
  uint8_t  mode;
  uint16_t groupIdFrom;
  uint8_t  sceneIdFrom;
  uint16_t groupIdTo;
  uint8_t  sceneIdTo;
} ZCL_CopyScene_t;

/**
 * \brief Add Scene Response Command Payload format.
 */

typedef struct PACK
{
  uint8_t status;
  uint16_t groupId;
  uint8_t sceneId;
} ZCL_AddSceneResponse_t;

/**
 * \brief View Scene Response Command Payload format.
 */

typedef struct PACK
{
  uint8_t status;
  uint16_t groupId;
  uint8_t sceneId;
  uint16_t transitionTime;
  uint8_t nameAndExtField[1]; // string sceneName, extension field.
} ZCL_ViewSceneResponse_t;

/**
 * \brief Remove Scene Command Payload format.
 */

typedef struct PACK
{
  uint8_t status;
  uint16_t groupId;
  uint8_t sceneId;
} ZCL_RemoveSceneResponse_t;

/**
 * \brief Remove All Scenes Response Command Payload format.
 */

typedef struct PACK
{
  uint8_t status;
  uint16_t groupId;
} ZCL_RemoveAllScenesResponse_t;

/**
 * \brief Store Scene Response Command Payload format.
 */

typedef struct PACK
{
  uint8_t status;
  uint16_t groupId;
  uint8_t sceneId;
} ZCL_StoreSceneResponse_t;

/**
 * \brief Get Scene Membership Response Command Payload format.
 */

typedef struct PACK
{
  uint8_t status;
  uint8_t capacity;
  uint16_t groupId;
  uint8_t sceneCount;
  uint8_t sceneList[16];
} ZCL_GetSceneMembershipResponse_t;

/**
 * \brief Enhanced Add Scene Response Command Payload format.
 */

typedef struct PACK
{
  uint8_t status;
  uint16_t groupId;
  uint8_t sceneId;
} ZCL_EnhancedAddSceneResponse_t;

/**
 * \brief Enhanced View Scene Response Command Payload format.
 */

typedef struct PACK
{
  uint8_t status;
  uint16_t groupId;
  uint8_t sceneId;
  uint16_t transitionTime;
  uint8_t name[1];
  ZCL_OnOffClusterExtensionFieldSet_t onOffClusterExtFields;
  ZCL_LevelControlClusterExtensionFieldSet_t levelControlClusterExtFields;
  ZCL_ColorControlClusterExtensionFieldSet_t colorControlClusterExtFields;
} ZCL_EnhancedViewSceneResponse_t;

/**
 * \brief Copy Scene Response Command Payload format.
 */

typedef struct PACK
{
  uint8_t  status;
  uint16_t groupIdFrom;
  uint8_t  sceneIdFrom;
} ZCL_CopySceneResponse_t;

/**
 * \brief Extension Fields Set format
 */

typedef struct PACK
{
  uint16_t clusterId;
  uint8_t length;
} ZCL_ExtensionFieldSets_t;

/**
 * \brief Scene Cluster server's attributes
 */

typedef struct PACK
{
<#if (SCENES_CLUSTER_CS != "CLIENT")  && (SC_SCENECOUNT == true) >
  struct PACK
  {
    ZCL_AttributeId_t id;
    uint8_t type;
    uint8_t properties;
    uint8_t value;
  } sceneCount;
</#if>
<#if (SCENES_CLUSTER_CS != "CLIENT")  && (SC_CURRENTSCENE == true) >
  struct PACK
  {
    ZCL_AttributeId_t id;
    uint8_t type;
    uint8_t properties;
    uint8_t value;
  } currentScene;
</#if>
<#if (SCENES_CLUSTER_CS != "CLIENT")  && (SC_CURRENTGROUP == true) >
  struct PACK
  {
    ZCL_AttributeId_t id;
    uint8_t type;
    uint8_t properties;
    uint16_t value;
    uint16_t minVal;
    uint16_t maxVal;
  } currentGroup;
</#if>
<#if (SCENES_CLUSTER_CS != "CLIENT")  && (SC_SCENEVALID == true) >
  struct PACK
  {
    ZCL_AttributeId_t id;
    uint8_t type;
    uint8_t properties;
    bool value;
  } sceneValid;
</#if>
<#if (SCENES_CLUSTER_CS != "CLIENT")  && (SC_SSNAMESUPPORT == true) >
  struct PACK
  {
    ZCL_AttributeId_t id;
    uint8_t type;
    uint8_t properties;
    uint8_t value;
  } nameSupport;
</#if>
<#if (SCENES_CLUSTER_CS != "CLIENT")  && (SCS_CLUSTERREVISION == true) >
  struct PACK
  {
    ZCL_AttributeId_t id;
    uint8_t type;
    uint8_t properties;
    uint16_t value;
  } clusterVersion;
</#if>
} ZCL_SceneClusterServerAttributes_t;

/**
 * \brief Scene Cluster client's attributes
 */

typedef struct PACK
{
<#if (SCENES_CLUSTER_CS != "SERVER")  && (SCC_CLUSTERREVISION == true) >  
  struct PACK
  {
    ZCL_AttributeId_t id;
    uint8_t type;
    uint8_t properties;
    uint16_t value;
  } clusterVersion;
</#if>
} ZCL_SceneClusterClientAttributes_t;

END_PACK

/**
 * \brief Scene Cluster commands.
 */

typedef struct
{
<#if (SCENES_CLUSTER_CS != "CLIENT")  && (SC_ADDSCENE == true) >  
  struct
  {
    ZCL_CommandId_t id;
    ZclCommandOptions_t options;
    ZCL_Status_t(*addSceneCommand)(ZCL_Addressing_t *addressing, uint8_t payloadLength, ZCL_AddScene_t * payload);
  } addSceneCommand;
</#if>
<#if (SCENES_CLUSTER_CS != "CLIENT")  && (SC_VIEWSCENE == true) >
  struct
  {
    ZCL_CommandId_t id;
    ZclCommandOptions_t options;
    ZCL_Status_t(*viewSceneCommand)(ZCL_Addressing_t *addressing, uint8_t payloadLength, ZCL_ViewScene_t * payload);
  } viewSceneCommand;
</#if>
<#if (SCENES_CLUSTER_CS != "CLIENT")  && (SC_REMOVESCENE == true) >
  struct
  {
    ZCL_CommandId_t id;
    ZclCommandOptions_t options;
    ZCL_Status_t(*removeSceneCommand)(ZCL_Addressing_t *addressing, uint8_t payloadLength, ZCL_RemoveScene_t * payload);
  } removeSceneCommand;
</#if>
<#if (SCENES_CLUSTER_CS != "CLIENT")  && (SC_REMOVESCENEALL == true) >
  struct
  {
    ZCL_CommandId_t id;
    ZclCommandOptions_t options;
    ZCL_Status_t(*removeAllScenesCommand)(ZCL_Addressing_t *addressing, uint8_t payloadLength, ZCL_RemoveAllScenes_t * payload);
  } removeAllScenesCommand;
</#if>
<#if (SCENES_CLUSTER_CS != "CLIENT")  && (SC_STORESCENE == true) >
  struct
  {
    ZCL_CommandId_t id;
    ZclCommandOptions_t options;
    ZCL_Status_t(*storeSceneCommand)(ZCL_Addressing_t *addressing, uint8_t payloadLength, ZCL_StoreScene_t * payload);
  } storeSceneCommand;
</#if>
<#if (SCENES_CLUSTER_CS != "CLIENT")  && (SC_RECALLSCENE == true) >
  struct
  {
    ZCL_CommandId_t id;
    ZclCommandOptions_t options;
    ZCL_Status_t(*recallSceneCommand)(ZCL_Addressing_t *addressing, uint8_t payloadLength, ZCL_RecallScene_t * payload);
  } recallSceneCommand;
</#if>
<#if (SCENES_CLUSTER_CS != "CLIENT")  && (SC_SCENEMEM == true) >
  struct
  {
    ZCL_CommandId_t id;
    ZclCommandOptions_t options;
    ZCL_Status_t(*getSceneMembershipCommand)(ZCL_Addressing_t *addressing, uint8_t payloadLength, ZCL_GetSceneMembership_t * payload);
  } getSceneMembershipCommand;
</#if>
<#if (SCENES_CLUSTER_CS != "CLIENT")  && (SC_ENHANCEDADDSCENE == true) >
  struct
  {
    ZCL_CommandId_t id;
    ZclCommandOptions_t options;
    ZCL_Status_t(*enhancedAddSceneCommand)(ZCL_Addressing_t *addressing, uint8_t payloadLength, ZCL_EnhancedAddScene_t * payload);
  } enhancedAddSceneCommand;
</#if>
<#if (SCENES_CLUSTER_CS != "CLIENT")  && (SC_ENHANCEDVIEWSCENE == true) >
  struct
  {
    ZCL_CommandId_t id;
    ZclCommandOptions_t options;
    ZCL_Status_t(*enhancedViewSceneCommand)(ZCL_Addressing_t *addressing, uint8_t payloadLength, ZCL_EnhancedViewScene_t * payload);
  } enhancedViewSceneCommand;
</#if>
<#if (SCENES_CLUSTER_CS != "CLIENT")  && (SC_COPYSCENE == true) >
  struct
  {
    ZCL_CommandId_t id;
    ZclCommandOptions_t options;
    ZCL_Status_t(*copySceneCommand)(ZCL_Addressing_t *addressing, uint8_t payloadLength, ZCL_CopyScene_t * payload);
  } copySceneCommand;
</#if>
  // responses
<#if (SCENES_CLUSTER_CS != "SERVER")  && (SCC_ADDSCENERESP == true) >
  struct
  {
    ZCL_CommandId_t id;
    ZclCommandOptions_t options;
    ZCL_Status_t(*addSceneResponseCommand)(ZCL_Addressing_t *addressing, uint8_t payloadLength, ZCL_AddSceneResponse_t * payload);
  } addSceneResponseCommand;
</#if>
<#if (SCENES_CLUSTER_CS != "SERVER")  && (SCC_VIEWSCENERES == true) >
  struct
  {
    ZCL_CommandId_t id;
    ZclCommandOptions_t options;
    ZCL_Status_t(*viewSceneResponseCommand)(ZCL_Addressing_t *addressing, uint8_t payloadLength, ZCL_ViewSceneResponse_t * payload);
  } viewSceneResponseCommand;
</#if>
<#if (SCENES_CLUSTER_CS != "SERVER")  && (SCC_REMOVESCENERES == true) >
  struct
  {
    ZCL_CommandId_t id;
    ZclCommandOptions_t options;
    ZCL_Status_t(*removeSceneResponseCommand)(ZCL_Addressing_t *addressing, uint8_t payloadLength, ZCL_RemoveSceneResponse_t * payload);
  } removeSceneResponseCommand;
</#if>
<#if (SCENES_CLUSTER_CS != "SERVER")  && (SCC_REMOVESCENEALLRES == true) >
  struct
  {
    ZCL_CommandId_t id;
    ZclCommandOptions_t options;
    ZCL_Status_t(*removeAllScenesResponseCommand)(ZCL_Addressing_t *addressing, uint8_t payloadLength, ZCL_RemoveAllScenesResponse_t * payload);
  } removeAllScenesResponseCommand;
</#if>
<#if (SCENES_CLUSTER_CS != "SERVER")  && (SCC_STORESCENERES == true) >
  struct
  {
    ZCL_CommandId_t id;
    ZclCommandOptions_t options;
    ZCL_Status_t(*storeSceneResponseCommand)(ZCL_Addressing_t *addressing, uint8_t payloadLength, ZCL_StoreSceneResponse_t * payload);
  } storeSceneResponseCommand;
</#if>
<#if (SCENES_CLUSTER_CS != "SERVER")  && (SCC_SCENEMEMRES == true) >
  struct
  {
    ZCL_CommandId_t id;
    ZclCommandOptions_t options;
    ZCL_Status_t(*getSceneMembershipResponseCommand)(ZCL_Addressing_t *addressing, uint8_t payloadLength, ZCL_GetSceneMembershipResponse_t * payload);
  } getSceneMembershipResponseCommand;
</#if>
<#if (SCENES_CLUSTER_CS != "SERVER")  && (SCC_ENHANCEDADDSCENERES == true) >
  struct
  {
    ZCL_CommandId_t id;
    ZclCommandOptions_t options;
    ZCL_Status_t(*enhancedAddSceneResponseCommand)(ZCL_Addressing_t *addressing, uint8_t payloadLength, ZCL_EnhancedAddSceneResponse_t * payload);
  } enhancedAddSceneResponseCommand;
</#if>
<#if (SCENES_CLUSTER_CS != "SERVER")  && (SCC_ENHANCEDVIEWSCENERES == true) >
  struct
  {
    ZCL_CommandId_t id;
    ZclCommandOptions_t options;
    ZCL_Status_t(*enhancedViewSceneResponseCommand)(ZCL_Addressing_t *addressing, uint8_t payloadLength, ZCL_EnhancedViewSceneResponse_t * payload);
  } enhancedViewSceneResponseCommand;
</#if>
<#if (SCENES_CLUSTER_CS != "SERVER")  && (SCC_COPYSCENERES == true) >
  struct
  {
    ZCL_CommandId_t id;
    ZclCommandOptions_t options;
    ZCL_Status_t(*copySceneResponseCommand)(ZCL_Addressing_t *addressing, uint8_t payloadLength, ZCL_CopySceneResponse_t * payload);
  } copySceneResponseCommand;
</#if>
} ZCL_ScenesClusterCommands_t;


#endif /* _ZCLSCENESCLUSTER_H */