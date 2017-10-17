    
    #include "threads.h"

    void threads_multiple_start(aobjects * aobjs) {

        unsigned int iSink;

        // +----------------------------------------------------------+
        // | Raw                                                      |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Source                                               |
            // +------------------------------------------------------+  

                thread_start(aobjs->asrc_hops_mics_object->thread);

            // +------------------------------------------------------+
            // | Connector                                            |
            // +------------------------------------------------------+                     

                thread_start(aobjs->acon_hops_mics_raw_object->thread);

        // +----------------------------------------------------------+
        // | Mapping                                                  |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Module                                               |
            // +------------------------------------------------------+  

                thread_start(aobjs->amod_mapping_mics_object->thread);

            // +------------------------------------------------------+
            // | Connector                                            |
            // +------------------------------------------------------+                     

                thread_start(aobjs->acon_hops_mics_map_object->thread);

        // +----------------------------------------------------------+
        // | Resample                                                 |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Module                                               |
            // +------------------------------------------------------+  

                thread_start(aobjs->amod_resample_mics_object->thread);

            // +------------------------------------------------------+
            // | Connector                                            |
            // +------------------------------------------------------+                     

                thread_start(aobjs->acon_hops_mics_rs_object->thread);

        // +----------------------------------------------------------+
        // | STFT                                                     |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Module                                               |
            // +------------------------------------------------------+  

                thread_start(aobjs->amod_stft_mics_object->thread);

            // +------------------------------------------------------+
            // | Connector                                            |
            // +------------------------------------------------------+                     

                thread_start(aobjs->acon_spectra_mics_object->thread);

        // +----------------------------------------------------------+
        // | SSL                                                      |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Module                                               |
            // +------------------------------------------------------+  

                thread_start(aobjs->amod_ssl_object->thread);

            // +------------------------------------------------------+
            // | Sinks                                                |
            // +------------------------------------------------------+                      

                thread_start(aobjs->asnk_pots_ssl_object->thread);

            // +------------------------------------------------------+
            // | Connector                                            |
            // +------------------------------------------------------+                     

                thread_start(aobjs->acon_pots_ssl_object->thread);

        // +----------------------------------------------------------+
        // | SST                                                      |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Module                                               |
            // +------------------------------------------------------+  

                thread_start(aobjs->amod_sst_object->thread);

            // +------------------------------------------------------+
            // | Sinks                                                |
            // +------------------------------------------------------+                      

                thread_start(aobjs->asnk_tracks_sst_object->thread);

            // +------------------------------------------------------+
            // | Connector                                            |
            // +------------------------------------------------------+                     

                thread_start(aobjs->acon_tracks_sst_object->thread);

        // +----------------------------------------------------------+
        // | SSS                                                      |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Module                                               |
            // +------------------------------------------------------+  

                thread_start(aobjs->amod_sss_object->thread);

            // +------------------------------------------------------+
            // | Connector                                            |
            // +------------------------------------------------------+                     

                thread_start(aobjs->acon_spectra_seps_object->thread);
                thread_start(aobjs->acon_spectra_pfs_object->thread);

        // +----------------------------------------------------------+
        // | ISTFT                                                    |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Module                                               |
            // +------------------------------------------------------+  

                thread_start(aobjs->amod_istft_seps_object->thread);
                thread_start(aobjs->amod_istft_pfs_object->thread);

            // +------------------------------------------------------+
            // | Connector                                            |
            // +------------------------------------------------------+  

                thread_start(aobjs->acon_hops_seps_object->thread);
                thread_start(aobjs->acon_hops_pfs_object->thread);

        // +------------------------------------------------------+
        // | Resample                                             |
        // +------------------------------------------------------+  

            // +--------------------------------------------------+
            // | Module                                           |
            // +--------------------------------------------------+  

                thread_start(aobjs->amod_resample_seps_object->thread);
                thread_start(aobjs->amod_resample_pfs_object->thread);

            // +--------------------------------------------------+
            // | Sinks                                            |
            // +--------------------------------------------------+                     

                thread_start(aobjs->asnk_hops_seps_rs_object->thread);
                thread_start(aobjs->asnk_hops_pfs_rs_object->thread);

            // +--------------------------------------------------+
            // | Connector                                        |
            // +--------------------------------------------------+  

                thread_start(aobjs->acon_hops_seps_rs_object->thread);
                thread_start(aobjs->acon_hops_pfs_rs_object->thread);

    }

    void threads_multiple_stop(aobjects * aobjs) {

        // +----------------------------------------------------------+
        // | Raw                                                      |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Source                                               |
            // +------------------------------------------------------+  

                thread_stop(aobjs->asrc_hops_mics_object->thread);

                // Stops the source that generate frames to process.
                // This will generate a "zero" message that will stop
                // all following threads.

    }

    void threads_multiple_join(aobjects * aobjs) {

        unsigned int iSink;

        // +----------------------------------------------------------+
        // | Raw                                                      |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Source                                               |
            // +------------------------------------------------------+  

                thread_join(aobjs->asrc_hops_mics_object->thread);

            // +------------------------------------------------------+
            // | Connector                                            |
            // +------------------------------------------------------+  

                thread_join(aobjs->acon_hops_mics_raw_object->thread);

        // +----------------------------------------------------------+
        // | Mapping                                                  |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Module                                               |
            // +------------------------------------------------------+  

                thread_join(aobjs->amod_mapping_mics_object->thread);

            // +------------------------------------------------------+
            // | Connector                                            |
            // +------------------------------------------------------+  

                thread_join(aobjs->acon_hops_mics_map_object->thread);

        // +----------------------------------------------------------+
        // | Resample                                                 |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Module                                               |
            // +------------------------------------------------------+  

                thread_join(aobjs->amod_resample_mics_object->thread);               

            // +------------------------------------------------------+
            // | Connector                                            |
            // +------------------------------------------------------+  

                thread_join(aobjs->acon_hops_mics_rs_object->thread);

        // +----------------------------------------------------------+
        // | STFT                                                     |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Module                                               |
            // +------------------------------------------------------+  

                thread_join(aobjs->amod_stft_mics_object->thread);               

            // +------------------------------------------------------+
            // | Connector                                            |
            // +------------------------------------------------------+  

                thread_join(aobjs->acon_spectra_mics_object->thread);

        // +----------------------------------------------------------+
        // | SSL                                                      |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Module                                               |
            // +------------------------------------------------------+  

                thread_join(aobjs->amod_ssl_object->thread);               

            // +------------------------------------------------------+
            // | Sinks                                                |
            // +------------------------------------------------------+   
                
                thread_join(aobjs->asnk_pots_ssl_object->thread);

            // +------------------------------------------------------+
            // | Connector                                            |
            // +------------------------------------------------------+  

                thread_join(aobjs->acon_pots_ssl_object->thread);

        // +----------------------------------------------------------+
        // | SST                                                      |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Module                                               |
            // +------------------------------------------------------+  

                thread_join(aobjs->amod_sst_object->thread);               

            // +------------------------------------------------------+
            // | Sinks                                                |
            // +------------------------------------------------------+   
                
                thread_join(aobjs->asnk_tracks_sst_object->thread);

            // +------------------------------------------------------+
            // | Connector                                            |
            // +------------------------------------------------------+  

                thread_join(aobjs->acon_tracks_sst_object->thread);

        // +----------------------------------------------------------+
        // | SSS                                                      |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Module                                               |
            // +------------------------------------------------------+  

                thread_join(aobjs->amod_sss_object->thread);               

            // +------------------------------------------------------+
            // | Connector                                            |
            // +------------------------------------------------------+  

                thread_join(aobjs->acon_spectra_seps_object->thread);
                thread_join(aobjs->acon_spectra_pfs_object->thread);

        // +----------------------------------------------------------+
        // | ISTFT                                                    |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Module                                               |
            // +------------------------------------------------------+  

                thread_join(aobjs->amod_istft_seps_object->thread);
                thread_join(aobjs->amod_istft_pfs_object->thread);

            // +------------------------------------------------------+
            // | Connector                                            |
            // +------------------------------------------------------+  

                thread_join(aobjs->acon_hops_seps_object->thread);
                thread_join(aobjs->acon_hops_pfs_object->thread);

        // +------------------------------------------------------+
        // | Resample                                             |
        // +------------------------------------------------------+  

            // +--------------------------------------------------+
            // | Module                                           |
            // +--------------------------------------------------+  

                thread_join(aobjs->amod_resample_seps_object->thread);
                thread_join(aobjs->amod_resample_pfs_object->thread);

            // +--------------------------------------------------+
            // | Sinks                                            |
            // +--------------------------------------------------+                     

                thread_join(aobjs->asnk_hops_seps_rs_object->thread);
                thread_join(aobjs->asnk_hops_pfs_rs_object->thread);

            // +--------------------------------------------------+
            // | Connector                                        |
            // +--------------------------------------------------+  

                thread_join(aobjs->acon_hops_seps_rs_object->thread);
                thread_join(aobjs->acon_hops_pfs_rs_object->thread);

    }

    void threads_single_open(objects * objs) {

        unsigned int iSink;

        // +----------------------------------------------------------+
        // | Raw                                                      |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Source                                               |
            // +------------------------------------------------------+  

                src_hops_open(objs->src_hops_mics_object);

        // +----------------------------------------------------------+
        // | SSL                                                      |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Sinks                                                |
            // +------------------------------------------------------+                      

                snk_pots_open(objs->snk_pots_ssl_object);

        // +----------------------------------------------------------+
        // | SST                                                      |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Sinks                                                |
            // +------------------------------------------------------+                      

                snk_tracks_open(objs->snk_tracks_sst_object);

        // +----------------------------------------------------------+
        // | Resample                                                 |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Sinks                                                |
            // +------------------------------------------------------+                      

                snk_hops_open(objs->snk_hops_seps_rs_object);
                snk_hops_open(objs->snk_hops_pfs_rs_object);

    }

    void threads_single_close(objects * objs) {

        unsigned int iSink;

        // +----------------------------------------------------------+
        // | Raw                                                      |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Source                                               |
            // +------------------------------------------------------+  

                src_hops_close(objs->src_hops_mics_object);

        // +----------------------------------------------------------+
        // | SSL                                                      |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Sinks                                                |
            // +------------------------------------------------------+                      

                snk_pots_close(objs->snk_pots_ssl_object);

        // +----------------------------------------------------------+
        // | SST                                                      |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Sinks                                                |
            // +------------------------------------------------------+                      

                snk_tracks_close(objs->snk_tracks_sst_object);

        // +----------------------------------------------------------+
        // | Resample                                                 |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Sinks                                                |
            // +------------------------------------------------------+                      

                snk_hops_close(objs->snk_hops_seps_rs_object);
                snk_hops_close(objs->snk_hops_pfs_rs_object);                

    }

    int threads_single_process(objects * objs, profiler * prf) {

        int rtnValue;
        int rtnResample;
        unsigned int iSink;

        clock_t begin, end;

        prf->duration += ((float) (objs->src_hops_mics_object->hopSize)) / ((float) (objs->src_hops_mics_object->fS));

        // +----------------------------------------------------------+
        // | SECTION I                                                |
        // +-----------------------------------------------------------

            // +------------------------------------------------------+
            // | Raw                                                  |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Source                                           |
                // +--------------------------------------------------+  

                    begin = clock();
                    rtnValue = src_hops_process(objs->src_hops_mics_object);
                    end = clock();
                    prf->src_hops_mics_prf += (float) (((double) (end-begin)) / CLOCKS_PER_SEC);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+   

                    begin = clock();
                    con_hops_process(objs->con_hops_mics_raw_object);
                    end = clock();
                    prf->con_hops_mics_raw_prf += (float) (((double) (end-begin)) / CLOCKS_PER_SEC);

            // +------------------------------------------------------+
            // | Mapping                                              |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    begin = clock();
                    mod_mapping_process(objs->mod_mapping_mics_object);
                    end = clock();
                    prf->mod_mapping_mics_prf += (float) (((double) (end-begin)) / CLOCKS_PER_SEC);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+                     

                    begin = clock();
                    con_hops_process(objs->con_hops_mics_map_object);
                    end = clock();
                    prf->con_hops_mics_map_prf += (float) (((double) (end-begin)) / CLOCKS_PER_SEC);

            // +------------------------------------------------------+
            // | Resample                                             |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    begin = clock();
                    mod_resample_process_push(objs->mod_resample_mics_object);
                    end = clock();
                    prf->mod_resample_mics_prf += (float) (((double) (end-begin)) / CLOCKS_PER_SEC);

        // +----------------------------------------------------------+
        // | SECTION II                                               |
        // +-----------------------------------------------------------

            // Loop through section II as long as there are frames to process from the resample module
            while(1) {

            // +------------------------------------------------------+
            // | Resample                                             |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    begin = clock();
                    rtnResample = mod_resample_process_pop(objs->mod_resample_mics_object);
                    end = clock();
                    prf->mod_resample_mics_prf += (float) (((double) (end-begin)) / CLOCKS_PER_SEC);

                    // If there is no frames to process, stop
                    if (rtnResample == -1) {
                        break;
                    }

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+      

                    begin = clock();
                    con_hops_process(objs->con_hops_mics_rs_object);
                    end = clock();
                    prf->con_hops_mics_rs_prf += (float) (((double) (end-begin)) / CLOCKS_PER_SEC);                

            // +------------------------------------------------------+
            // | STFT                                                 |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    begin = clock();
                    mod_stft_process(objs->mod_stft_mics_object);
                    end = clock();
                    prf->mod_stft_mics_prf += (float) (((double) (end-begin)) / CLOCKS_PER_SEC);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+                     

                    begin = clock();
                    con_spectra_process(objs->con_spectra_mics_object);
                    end = clock();
                    prf->con_spectra_mics_prf += (float) (((double) (end-begin)) / CLOCKS_PER_SEC);                     

            // +------------------------------------------------------+
            // | SSL                                                  |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    begin = clock();
                    mod_ssl_process(objs->mod_ssl_object);
                    end = clock();
                    prf->mod_ssl_prf += (float) (((double) (end-begin)) / CLOCKS_PER_SEC);                

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+                     

                    begin = clock();
                    con_pots_process(objs->con_pots_ssl_object);
                    end = clock();
                    prf->con_pots_ssl_prf += (float) (((double) (end-begin)) / CLOCKS_PER_SEC);                                    

                // +--------------------------------------------------+
                // | Sinks                                            |
                // +--------------------------------------------------+                      

                    begin = clock();
                    snk_pots_process(objs->snk_pots_ssl_object);
                    end = clock();
                    prf->snk_pots_ssl_prf += (float) (((double) (end-begin)) / CLOCKS_PER_SEC);                                                

            // +------------------------------------------------------+
            // | SST                                                  |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    begin = clock();
                    mod_sst_process(objs->mod_sst_object);
                    end = clock();
                    prf->mod_sst_prf += (float) (((double) (end-begin)) / CLOCKS_PER_SEC);                

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+                     

                    begin = clock();
                    con_tracks_process(objs->con_tracks_sst_object);
                    end = clock();
                    prf->con_tracks_sst_prf += (float) (((double) (end-begin)) / CLOCKS_PER_SEC);                                                   

                // +--------------------------------------------------+
                // | Sinks                                            |
                // +--------------------------------------------------+                      

                    begin = clock();
                    snk_tracks_process(objs->snk_tracks_sst_object);
                    end = clock();
                    prf->snk_tracks_sst_prf += (float) (((double) (end-begin)) / CLOCKS_PER_SEC);                                                   

            // +------------------------------------------------------+
            // | SSS                                                  |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    begin = clock();
                    mod_sss_process(objs->mod_sss_object);
                    end = clock();
                    prf->mod_sss_prf += (float) (((double) (end-begin)) / CLOCKS_PER_SEC);                

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+                     

                    begin = clock();
                    con_spectra_process(objs->con_spectra_seps_object);
                    end = clock();
                    prf->con_spectra_seps_prf += (float) (((double) (end-begin)) / CLOCKS_PER_SEC);  

                    begin = clock();
                    con_spectra_process(objs->con_spectra_pfs_object);
                    end = clock();
                    prf->con_spectra_pfs_prf += (float) (((double) (end-begin)) / CLOCKS_PER_SEC);                      

            // +------------------------------------------------------+
            // | ISTFT                                                |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    begin = clock();
                    mod_istft_process(objs->mod_istft_seps_object);
                    end = clock();
                    prf->mod_istft_seps_prf += (float) (((double) (end-begin)) / CLOCKS_PER_SEC);      

                    begin = clock();
                    mod_istft_process(objs->mod_istft_pfs_object);
                    end = clock();
                    prf->mod_istft_pfs_prf += (float) (((double) (end-begin)) / CLOCKS_PER_SEC);      

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    begin = clock();
                    con_hops_process(objs->con_hops_seps_object);
                    end = clock();
                    prf->con_hops_seps_prf += (float) (((double) (end-begin)) / CLOCKS_PER_SEC);      

                    begin = clock();
                    con_hops_process(objs->con_hops_pfs_object);
                    end = clock();
                    prf->con_hops_pfs_prf += (float) (((double) (end-begin)) / CLOCKS_PER_SEC);     

            // +------------------------------------------------------+
            // | Resample                                             |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    begin = clock();
                    mod_resample_process_push(objs->mod_resample_seps_object);
                    end = clock();
                    prf->mod_resample_seps_prf += (float) (((double) (end-begin)) / CLOCKS_PER_SEC);

                    begin = clock();
                    mod_resample_process_push(objs->mod_resample_pfs_object);
                    end = clock();
                    prf->mod_resample_pfs_prf += (float) (((double) (end-begin)) / CLOCKS_PER_SEC);

            // +------------------------------------------------------+
            // | SECTION III                                          |
            // +------------------------------------------------------+

                while(1) {

                    // +----------------------------------------------+
                    // | Module                                       |
                    // +----------------------------------------------+  

                        begin = clock();
                        rtnResample = mod_resample_process_pop(objs->mod_resample_seps_object);
                        end = clock();
                        prf->mod_resample_seps_prf += (float) (((double) (end-begin)) / CLOCKS_PER_SEC);

                        // If there is no frames to process, stop
                        if (rtnResample == -1) {
                            break;
                        }

                    // +----------------------------------------------+
                    // | Connector                                    |
                    // +----------------------------------------------+  

                        begin = clock();
                        con_hops_process(objs->con_hops_seps_rs_object);
                        end = clock();
                        prf->con_hops_seps_rs_prf += (float) (((double) (end-begin)) / CLOCKS_PER_SEC);

                    // +----------------------------------------------+
                    // | Sink                                         |
                    // +----------------------------------------------+  

                        begin = clock();
                        snk_hops_process(objs->snk_hops_seps_rs_object);
                        end = clock();
                        prf->snk_hops_seps_rs_prf += (float) (((double) (end-begin)) / CLOCKS_PER_SEC);

                    }


                while(1) {

                    // +----------------------------------------------+
                    // | Module                                       |
                    // +----------------------------------------------+  

                        begin = clock();
                        rtnResample = mod_resample_process_pop(objs->mod_resample_pfs_object);
                        end = clock();
                        prf->mod_resample_pfs_prf += (float) (((double) (end-begin)) / CLOCKS_PER_SEC);

                        // If there is no frames to process, stop
                        if (rtnResample == -1) {
                            break;
                        }

                    // +----------------------------------------------+
                    // | Connector                                    |
                    // +----------------------------------------------+  

                        begin = clock();
                        con_hops_process(objs->con_hops_pfs_rs_object);
                        end = clock();
                        prf->con_hops_pfs_rs_prf += (float) (((double) (end-begin)) / CLOCKS_PER_SEC);

                    // +----------------------------------------------+
                    // | Sink                                         |
                    // +----------------------------------------------+  

                        begin = clock();
                        snk_hops_process(objs->snk_hops_pfs_rs_object);
                        end = clock();
                        prf->snk_hops_pfs_rs_prf += (float) (((double) (end-begin)) / CLOCKS_PER_SEC);

                    }

            }

        return rtnValue;

    }