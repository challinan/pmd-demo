

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from pmd.idl using "rtiddsgen".
The rtiddsgen tool is part of the RTI Connext distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the RTI Connext manual.
*/

#ifndef NDDS_STANDALONE_TYPE
#ifndef ndds_cpp_h
#include "ndds/ndds_cpp.h"
#endif
#ifndef dds_c_log_impl_h              
#include "dds_c/dds_c_log_impl.h"                                
#endif        

#ifndef cdr_type_h
#include "cdr/cdr_type.h"
#endif    

#ifndef osapi_heap_h
#include "osapi/osapi_heap.h" 
#endif
#else
#include "ndds_standalone_type.h"
#endif

#include "pmd.h"

namespace DataBus {

    /* ========================================================================= */
    const char *pm_data_structTYPENAME = "DataBus::pm_data_struct";

    DDS_TypeCode* pm_data_struct_get_typecode()
    {
        static RTIBool is_initialized = RTI_FALSE;

        static DDS_TypeCode_Member pm_data_struct_g_tc_members[3]=
        {

            {
                (char *)"ecgValue",/* Member name */
                {
                    0,/* Representation ID */          
                    DDS_BOOLEAN_FALSE,/* Is a pointer? */
                    -1, /* Bitfield bits */
                    NULL/* Member type code is assigned later */
                },
                0, /* Ignored */
                0, /* Ignored */
                0, /* Ignored */
                NULL, /* Ignored */
                RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                DDS_PUBLIC_MEMBER,/* Member visibility */
                1,
                NULL/* Ignored */
            }, 
            {
                (char *)"abpValue",/* Member name */
                {
                    1,/* Representation ID */          
                    DDS_BOOLEAN_FALSE,/* Is a pointer? */
                    -1, /* Bitfield bits */
                    NULL/* Member type code is assigned later */
                },
                0, /* Ignored */
                0, /* Ignored */
                0, /* Ignored */
                NULL, /* Ignored */
                RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                DDS_PUBLIC_MEMBER,/* Member visibility */
                1,
                NULL/* Ignored */
            }, 
            {
                (char *)"plethValue",/* Member name */
                {
                    2,/* Representation ID */          
                    DDS_BOOLEAN_FALSE,/* Is a pointer? */
                    -1, /* Bitfield bits */
                    NULL/* Member type code is assigned later */
                },
                0, /* Ignored */
                0, /* Ignored */
                0, /* Ignored */
                NULL, /* Ignored */
                RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                DDS_PUBLIC_MEMBER,/* Member visibility */
                1,
                NULL/* Ignored */
            }
        };

        static DDS_TypeCode pm_data_struct_g_tc =
        {{
                DDS_TK_STRUCT,/* Kind */
                DDS_BOOLEAN_FALSE, /* Ignored */
                -1, /*Ignored*/
                (char *)"DataBus::pm_data_struct", /* Name */
                NULL, /* Ignored */      
                0, /* Ignored */
                0, /* Ignored */
                NULL, /* Ignored */
                3, /* Number of members */
                pm_data_struct_g_tc_members, /* Members */
                DDS_VM_NONE  /* Ignored */         
            }}; /* Type code for pm_data_struct*/

        if (is_initialized) {
            return &pm_data_struct_g_tc;
        }

        pm_data_struct_g_tc_members[0]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_long;

        pm_data_struct_g_tc_members[1]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_long;

        pm_data_struct_g_tc_members[2]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_long;

        is_initialized = RTI_TRUE;

        return &pm_data_struct_g_tc;
    }

    RTIBool pm_data_struct_initialize(
        pm_data_struct* sample) {
        return DataBus::pm_data_struct_initialize_ex(sample,RTI_TRUE,RTI_TRUE);
    }

    RTIBool pm_data_struct_initialize_ex(
        pm_data_struct* sample,RTIBool allocatePointers, RTIBool allocateMemory)
    {

        struct DDS_TypeAllocationParams_t allocParams =
        DDS_TYPE_ALLOCATION_PARAMS_DEFAULT;

        allocParams.allocate_pointers =  (DDS_Boolean)allocatePointers;
        allocParams.allocate_memory = (DDS_Boolean)allocateMemory;

        return DataBus::pm_data_struct_initialize_w_params(
            sample,&allocParams);

    }

    RTIBool pm_data_struct_initialize_w_params(
        pm_data_struct* sample, const struct DDS_TypeAllocationParams_t * allocParams)
    {

        if (allocParams) {} /* To avoid warnings */

        if (!RTICdrType_initLong(&sample->ecgValue)) {
            return RTI_FALSE;
        }     

        if (!RTICdrType_initLong(&sample->abpValue)) {
            return RTI_FALSE;
        }     

        if (!RTICdrType_initLong(&sample->plethValue)) {
            return RTI_FALSE;
        }     

        return RTI_TRUE;
    }

    void pm_data_struct_finalize(
        pm_data_struct* sample)
    {

        DataBus::pm_data_struct_finalize_ex(sample,RTI_TRUE);
    }

    void pm_data_struct_finalize_ex(
        pm_data_struct* sample,RTIBool deletePointers)
    {
        struct DDS_TypeDeallocationParams_t deallocParams =
        DDS_TYPE_DEALLOCATION_PARAMS_DEFAULT;

        if (sample==NULL) {
            return;
        } 

        deallocParams.delete_pointers = (DDS_Boolean)deletePointers;

        DataBus::pm_data_struct_finalize_w_params(
            sample,&deallocParams);
    }

    void pm_data_struct_finalize_w_params(
        pm_data_struct* sample,const struct DDS_TypeDeallocationParams_t * deallocParams)
    {

        if (sample==NULL) {
            return;
        }
        if (deallocParams) {} /* To avoid warnings */

    }

    void pm_data_struct_finalize_optional_members(
        pm_data_struct* sample, RTIBool deletePointers)
    {
        struct DDS_TypeDeallocationParams_t deallocParamsTmp =
        DDS_TYPE_DEALLOCATION_PARAMS_DEFAULT;
        struct DDS_TypeDeallocationParams_t * deallocParams =
        &deallocParamsTmp;

        if (sample==NULL) {
            return;
        } 
        if (deallocParams) {} /* To avoid warnings */

        deallocParamsTmp.delete_pointers = (DDS_Boolean)deletePointers;
        deallocParamsTmp.delete_optional_members = DDS_BOOLEAN_TRUE;

    }

    RTIBool pm_data_struct_copy(
        pm_data_struct* dst,
        const pm_data_struct* src)
    {

        if (!RTICdrType_copyLong (
            &dst->ecgValue, &src->ecgValue)) { 
            return RTI_FALSE;
        }
        if (!RTICdrType_copyLong (
            &dst->abpValue, &src->abpValue)) { 
            return RTI_FALSE;
        }
        if (!RTICdrType_copyLong (
            &dst->plethValue, &src->plethValue)) { 
            return RTI_FALSE;
        }

        return RTI_TRUE;
    }

    /**
    * <<IMPLEMENTATION>>
    *
    * Defines:  TSeq, T
    *
    * Configure and implement 'pm_data_struct' sequence class.
    */
    #define T pm_data_struct
    #define TSeq pm_data_structSeq
    #define T_initialize_w_params DataBus::pm_data_struct_initialize_w_params
    #define T_finalize_w_params   DataBus::pm_data_struct_finalize_w_params
    #define T_copy       DataBus::pm_data_struct_copy

    #ifndef NDDS_STANDALONE_TYPE
    #include "dds_c/generic/dds_c_sequence_TSeq.gen"
    #include "dds_cpp/generic/dds_cpp_sequence_TSeq.gen"
    #else
    #include "dds_c_sequence_TSeq.gen"
    #include "dds_cpp_sequence_TSeq.gen"
    #endif

    #undef T_copy
    #undef T_finalize_w_params
    #undef T_initialize_w_params
    #undef TSeq
    #undef T
} /* namespace DataBus  */

