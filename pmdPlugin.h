

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from pmd.idl using "rtiddsgen".
The rtiddsgen tool is part of the RTI Connext distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the RTI Connext manual.
*/

#ifndef pmdPlugin_464398885_h
#define pmdPlugin_464398885_h

#include "pmd.h"

struct RTICdrStream;

#ifndef pres_typePlugin_h
#include "pres/pres_typePlugin.h"
#endif

#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, start exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport __declspec(dllexport)
#endif

namespace DataBus {

    #define pm_data_structPlugin_get_sample PRESTypePluginDefaultEndpointData_getSample 
    #define pm_data_structPlugin_get_buffer PRESTypePluginDefaultEndpointData_getBuffer 
    #define pm_data_structPlugin_return_buffer PRESTypePluginDefaultEndpointData_returnBuffer 

    #define pm_data_structPlugin_create_sample PRESTypePluginDefaultEndpointData_createSample 
    #define pm_data_structPlugin_destroy_sample PRESTypePluginDefaultEndpointData_deleteSample 

    /* --------------------------------------------------------------------------------------
    Support functions:
    * -------------------------------------------------------------------------------------- */

    NDDSUSERDllExport extern pm_data_struct*
    pm_data_structPluginSupport_create_data_w_params(
        const struct DDS_TypeAllocationParams_t * alloc_params);

    NDDSUSERDllExport extern pm_data_struct*
    pm_data_structPluginSupport_create_data_ex(RTIBool allocate_pointers);

    NDDSUSERDllExport extern pm_data_struct*
    pm_data_structPluginSupport_create_data(void);

    NDDSUSERDllExport extern RTIBool 
    pm_data_structPluginSupport_copy_data(
        pm_data_struct *out,
        const pm_data_struct *in);

    NDDSUSERDllExport extern void 
    pm_data_structPluginSupport_destroy_data_w_params(
        pm_data_struct *sample,
        const struct DDS_TypeDeallocationParams_t * dealloc_params);

    NDDSUSERDllExport extern void 
    pm_data_structPluginSupport_destroy_data_ex(
        pm_data_struct *sample,RTIBool deallocate_pointers);

    NDDSUSERDllExport extern void 
    pm_data_structPluginSupport_destroy_data(
        pm_data_struct *sample);

    NDDSUSERDllExport extern void 
    pm_data_structPluginSupport_print_data(
        const pm_data_struct *sample,
        const char *desc,
        unsigned int indent);

    /* ----------------------------------------------------------------------------
    Callback functions:
    * ---------------------------------------------------------------------------- */

    NDDSUSERDllExport extern PRESTypePluginParticipantData 
    pm_data_structPlugin_on_participant_attached(
        void *registration_data, 
        const struct PRESTypePluginParticipantInfo *participant_info,
        RTIBool top_level_registration, 
        void *container_plugin_context,
        RTICdrTypeCode *typeCode);

    NDDSUSERDllExport extern void 
    pm_data_structPlugin_on_participant_detached(
        PRESTypePluginParticipantData participant_data);

    NDDSUSERDllExport extern PRESTypePluginEndpointData 
    pm_data_structPlugin_on_endpoint_attached(
        PRESTypePluginParticipantData participant_data,
        const struct PRESTypePluginEndpointInfo *endpoint_info,
        RTIBool top_level_registration, 
        void *container_plugin_context);

    NDDSUSERDllExport extern void 
    pm_data_structPlugin_on_endpoint_detached(
        PRESTypePluginEndpointData endpoint_data);

    NDDSUSERDllExport extern void    
    pm_data_structPlugin_return_sample(
        PRESTypePluginEndpointData endpoint_data,
        pm_data_struct *sample,
        void *handle);    

    NDDSUSERDllExport extern RTIBool 
    pm_data_structPlugin_copy_sample(
        PRESTypePluginEndpointData endpoint_data,
        pm_data_struct *out,
        const pm_data_struct *in);

    /* ----------------------------------------------------------------------------
    (De)Serialize functions:
    * ------------------------------------------------------------------------- */

    NDDSUSERDllExport extern RTIBool 
    pm_data_structPlugin_serialize(
        PRESTypePluginEndpointData endpoint_data,
        const pm_data_struct *sample,
        struct RTICdrStream *stream, 
        RTIBool serialize_encapsulation,
        RTIEncapsulationId encapsulation_id,
        RTIBool serialize_sample, 
        void *endpoint_plugin_qos);

    NDDSUSERDllExport extern RTIBool 
    pm_data_structPlugin_deserialize_sample(
        PRESTypePluginEndpointData endpoint_data,
        pm_data_struct *sample, 
        struct RTICdrStream *stream,
        RTIBool deserialize_encapsulation,
        RTIBool deserialize_sample, 
        void *endpoint_plugin_qos);

    NDDSUSERDllExport extern RTIBool
    pm_data_structPlugin_serialize_to_cdr_buffer(
        char * buffer,
        unsigned int * length,
        const pm_data_struct *sample); 

    NDDSUSERDllExport extern RTIBool 
    pm_data_structPlugin_deserialize(
        PRESTypePluginEndpointData endpoint_data,
        pm_data_struct **sample, 
        RTIBool * drop_sample,
        struct RTICdrStream *stream,
        RTIBool deserialize_encapsulation,
        RTIBool deserialize_sample, 
        void *endpoint_plugin_qos);

    NDDSUSERDllExport extern RTIBool
    pm_data_structPlugin_deserialize_from_cdr_buffer(
        pm_data_struct *sample,
        const char * buffer,
        unsigned int length);    

    NDDSUSERDllExport extern RTIBool
    pm_data_structPlugin_skip(
        PRESTypePluginEndpointData endpoint_data,
        struct RTICdrStream *stream, 
        RTIBool skip_encapsulation,  
        RTIBool skip_sample, 
        void *endpoint_plugin_qos);

    NDDSUSERDllExport extern unsigned int 
    pm_data_structPlugin_get_serialized_sample_max_size_ex(
        PRESTypePluginEndpointData endpoint_data,
        RTIBool * overflow,
        RTIBool include_encapsulation,
        RTIEncapsulationId encapsulation_id,
        unsigned int current_alignment);    

    NDDSUSERDllExport extern unsigned int 
    pm_data_structPlugin_get_serialized_sample_max_size(
        PRESTypePluginEndpointData endpoint_data,
        RTIBool include_encapsulation,
        RTIEncapsulationId encapsulation_id,
        unsigned int current_alignment);

    NDDSUSERDllExport extern unsigned int 
    pm_data_structPlugin_get_serialized_sample_min_size(
        PRESTypePluginEndpointData endpoint_data,
        RTIBool include_encapsulation,
        RTIEncapsulationId encapsulation_id,
        unsigned int current_alignment);

    NDDSUSERDllExport extern unsigned int
    pm_data_structPlugin_get_serialized_sample_size(
        PRESTypePluginEndpointData endpoint_data,
        RTIBool include_encapsulation,
        RTIEncapsulationId encapsulation_id,
        unsigned int current_alignment,
        const pm_data_struct * sample);

    /* --------------------------------------------------------------------------------------
    Key Management functions:
    * -------------------------------------------------------------------------------------- */
    NDDSUSERDllExport extern PRESTypePluginKeyKind 
    pm_data_structPlugin_get_key_kind(void);

    NDDSUSERDllExport extern unsigned int 
    pm_data_structPlugin_get_serialized_key_max_size_ex(
        PRESTypePluginEndpointData endpoint_data,
        RTIBool * overflow,
        RTIBool include_encapsulation,
        RTIEncapsulationId encapsulation_id,
        unsigned int current_alignment);

    NDDSUSERDllExport extern unsigned int 
    pm_data_structPlugin_get_serialized_key_max_size(
        PRESTypePluginEndpointData endpoint_data,
        RTIBool include_encapsulation,
        RTIEncapsulationId encapsulation_id,
        unsigned int current_alignment);

    NDDSUSERDllExport extern RTIBool 
    pm_data_structPlugin_serialize_key(
        PRESTypePluginEndpointData endpoint_data,
        const pm_data_struct *sample,
        struct RTICdrStream *stream,
        RTIBool serialize_encapsulation,
        RTIEncapsulationId encapsulation_id,
        RTIBool serialize_key,
        void *endpoint_plugin_qos);

    NDDSUSERDllExport extern RTIBool 
    pm_data_structPlugin_deserialize_key_sample(
        PRESTypePluginEndpointData endpoint_data,
        pm_data_struct * sample,
        struct RTICdrStream *stream,
        RTIBool deserialize_encapsulation,
        RTIBool deserialize_key,
        void *endpoint_plugin_qos);

    NDDSUSERDllExport extern RTIBool 
    pm_data_structPlugin_deserialize_key(
        PRESTypePluginEndpointData endpoint_data,
        pm_data_struct ** sample,
        RTIBool * drop_sample,
        struct RTICdrStream *stream,
        RTIBool deserialize_encapsulation,
        RTIBool deserialize_key,
        void *endpoint_plugin_qos);

    NDDSUSERDllExport extern RTIBool
    pm_data_structPlugin_serialized_sample_to_key(
        PRESTypePluginEndpointData endpoint_data,
        pm_data_struct *sample,
        struct RTICdrStream *stream, 
        RTIBool deserialize_encapsulation,  
        RTIBool deserialize_key, 
        void *endpoint_plugin_qos);

    /* Plugin Functions */
    NDDSUSERDllExport extern struct PRESTypePlugin*
    pm_data_structPlugin_new(void);

    NDDSUSERDllExport extern void
    pm_data_structPlugin_delete(struct PRESTypePlugin *);

} /* namespace DataBus  */

#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, stop exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif

#endif /* pmdPlugin_464398885_h */

