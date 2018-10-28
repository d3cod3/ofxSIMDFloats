

#include "allocator.h"

#include "ofConstants.h"
 
    
#ifdef OFX_SIMD_USE_SIMD
    //-------------------------ALIGNED ALLOCATION------------------------------------------
    void ofx_allocate_aligned(float* &toAllocate, int len){
        int check=0;
    #ifdef _MSC_VER
        toAllocate = reinterpret_cast<float*>(_aligned_malloc(len*sizeof(float), OFX_SIMD_ALIGNMENT_NUM));
        if(toAllocate ==NULL) {
                check = -1;
        }
    #else
        #ifdef TARGET_WIN32
            toAllocate = reinterpret_cast<float*>(_aligned_malloc(len*sizeof(float), OFX_SIMD_ALIGNMENT_NUM));
            if(toAllocate ==NULL) {
                check = -1;
            }
        #elif defined(TARGET_OSX) || defined(TARGET_LINUX)
            check = posix_memalign(reinterpret_cast<void**>(&toAllocate), OFX_SIMD_ALIGNMENT_NUM, len*sizeof(float));
        #endif
    #endif
                                
        if(check!=0){
                toAllocate = nullptr;
        }else{
                ofx_Aeq_Zero(toAllocate, len);
        }
    };
        
        
    void ofx_deallocate_aligned(float* &toDelete){

    #ifdef _MSC_VER
        _aligned_free(reinterpret_cast<void*>(toDelete));
    #else
        free(reinterpret_cast<void*>(toDelete));
    #endif
        toDelete = nullptr;
    }


#else //if we don't have NEON or SSE--------------------------------------------------------------------------------
                                            
                                                
    void ofx_allocate_aligned(float* &toAllocate, int len){
        toAllocate = (new float[len]);
        //need try / catch for memory errors
        for(int i = 0; i < len; ++i){
            toAllocate[i] = 0.0;
        }
    }

    void ofx_deallocate_aligned(float* &toDelete){
        delete [] toDelete;
        toDelete = nullptr;
    }

#endif
    
    
 
