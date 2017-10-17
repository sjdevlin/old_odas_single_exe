
    #include "configs.h"

    configs * configs_construct(const char * file_config) {

        configs * cfgs;
        unsigned int iSink;

        cfgs = (configs *) malloc(sizeof(configs));

        // +----------------------------------------------------------+
        // | Raw                                                      |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Source                                               |
            // +------------------------------------------------------+  

                cfgs->src_hops_mics_config = parameters_src_hops_mics_config(file_config);

            // +------------------------------------------------------+
            // | Message                                              |
            // +------------------------------------------------------+  

                cfgs->msg_hops_mics_raw_config = parameters_msg_hops_mics_raw_config(file_config);

        // +----------------------------------------------------------+
        // | Mapping                                                  |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Module                                               |
            // +------------------------------------------------------+  

                cfgs->mod_mapping_mics_config = parameters_mod_mapping_mics_config(file_config);

            // +------------------------------------------------------+
            // | Message                                              |
            // +------------------------------------------------------+  

                cfgs->msg_hops_mics_map_config = parameters_msg_hops_mics_map_config(file_config);


        // +----------------------------------------------------------+
        // | Resample                                                 |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Module                                               |
            // +------------------------------------------------------+  

                cfgs->mod_resample_mics_config = parameters_mod_resample_mics_config(file_config);

            // +------------------------------------------------------+
            // | Message                                              |
            // +------------------------------------------------------+  

                cfgs->msg_hops_mics_rs_config = parameters_msg_hops_mics_rs_config(file_config);

        // +----------------------------------------------------------+
        // | STFT                                                     |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Module                                               |
            // +------------------------------------------------------+  

                cfgs->mod_stft_mics_config = parameters_mod_stft_mics_config(file_config);

            // +------------------------------------------------------+
            // | Message                                              |
            // +------------------------------------------------------+  

                cfgs->msg_spectra_mics_config = parameters_msg_spectra_mics_config(file_config);

        // +----------------------------------------------------------+
        // | SSL                                                      |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Module                                               |
            // +------------------------------------------------------+  

                cfgs->mod_ssl_config = parameters_mod_ssl_config(file_config);

            // +------------------------------------------------------+
            // | Message                                              |
            // +------------------------------------------------------+  

                cfgs->msg_pots_ssl_config = parameters_msg_pots_ssl_config(file_config);

            // +------------------------------------------------------+
            // | Sinks                                                |
            // +------------------------------------------------------+  

                cfgs->snk_pots_ssl_config = parameters_snk_pots_ssl_config(file_config);

        // +----------------------------------------------------------+
        // | SST                                                      |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Module                                               |
            // +------------------------------------------------------+  

                cfgs->mod_sst_config = parameters_mod_sst_config(file_config);

            // +------------------------------------------------------+
            // | Message                                              |
            // +------------------------------------------------------+  

                cfgs->msg_tracks_sst_config = parameters_msg_tracks_sst_config(file_config);

            // +------------------------------------------------------+
            // | Sinks                                                |
            // +------------------------------------------------------+  

                cfgs->snk_tracks_sst_config = parameters_snk_tracks_sst_config(file_config);

        // +----------------------------------------------------------+
        // | SSS                                                      |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Module                                               |
            // +------------------------------------------------------+  

                cfgs->mod_sss_config = parameters_mod_sss_config(file_config);

            // +------------------------------------------------------+
            // | Message                                              |
            // +------------------------------------------------------+  

                cfgs->msg_spectra_seps_config = parameters_msg_spectra_seps_config(file_config);
                cfgs->msg_spectra_pfs_config = parameters_msg_spectra_pfs_config(file_config);

        // +----------------------------------------------------------+
        // | ISTFT                                                    |
        // +----------------------------------------------------------+

            // +------------------------------------------------------+
            // | Module                                               |
            // +------------------------------------------------------+

                cfgs->mod_istft_seps_config = parameters_mod_istft_seps_config(file_config);
                cfgs->mod_istft_pfs_config = parameters_mod_istft_pfs_config(file_config);

            // +------------------------------------------------------+
            // | Message                                              |
            // +------------------------------------------------------+

                cfgs->msg_hops_seps_config = parameters_msg_hops_seps_config(file_config);
                cfgs->msg_hops_pfs_config = parameters_msg_hops_pfs_config(file_config);

        // +----------------------------------------------------------+
        // | Resample                                                 |
        // +----------------------------------------------------------+

            // +------------------------------------------------------+
            // | Module                                               |
            // +------------------------------------------------------+

                cfgs->mod_resample_seps_config = parameters_mod_resample_seps_config(file_config);
                cfgs->mod_resample_pfs_config = parameters_mod_resample_pfs_config(file_config);

            // +------------------------------------------------------+
            // | Message                                              |
            // +------------------------------------------------------+

                cfgs->msg_hops_seps_rs_config = parameters_msg_hops_seps_rs_config(file_config);
                cfgs->msg_hops_pfs_rs_config = parameters_msg_hops_pfs_rs_config(file_config);            

            // +------------------------------------------------------+
            // | Sink                                                 |
            // +------------------------------------------------------+

                cfgs->snk_hops_seps_rs_config = parameters_snk_hops_seps_rs_config(file_config);
                cfgs->snk_hops_pfs_rs_config = parameters_snk_hops_pfs_rs_config(file_config);

        return cfgs;

    }

    void configs_destroy(configs * cfgs) {

        unsigned int iSink;

        // +----------------------------------------------------------+
        // | Raw                                                      |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Source                                               |
            // +------------------------------------------------------+  

                src_hops_cfg_destroy(cfgs->src_hops_mics_config);
                
            // +------------------------------------------------------+
            // | Message                                              |
            // +------------------------------------------------------+  

                msg_hops_cfg_destroy(cfgs->msg_hops_mics_raw_config);
                            
        // +----------------------------------------------------------+
        // | Mapping                                                  |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Module                                               |
            // +------------------------------------------------------+  

                mod_mapping_cfg_destroy(cfgs->mod_mapping_mics_config);
                
            // +------------------------------------------------------+
            // | Message                                              |
            // +------------------------------------------------------+  

                msg_hops_cfg_destroy(cfgs->msg_hops_mics_map_config);
                            
        // +----------------------------------------------------------+
        // | Resample                                                 |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Module                                               |
            // +------------------------------------------------------+  

                mod_resample_cfg_destroy(cfgs->mod_resample_mics_config);
                
            // +------------------------------------------------------+
            // | Message                                              |
            // +------------------------------------------------------+  

                msg_hops_cfg_destroy(cfgs->msg_hops_mics_rs_config);
  
        // +----------------------------------------------------------+
        // | STFT                                                     |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Module                                               |
            // +------------------------------------------------------+  

                mod_stft_cfg_destroy(cfgs->mod_stft_mics_config);
                
            // +------------------------------------------------------+
            // | Message                                              |
            // +------------------------------------------------------+  

                msg_spectra_cfg_destroy(cfgs->msg_spectra_mics_config);
                            
        // +----------------------------------------------------------+
        // | SSL                                                      |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Module                                               |
            // +------------------------------------------------------+  

                mod_ssl_cfg_destroy(cfgs->mod_ssl_config);
                
            // +------------------------------------------------------+
            // | Message                                              |
            // +------------------------------------------------------+  

                msg_pots_cfg_destroy(cfgs->msg_pots_ssl_config);
                            
            // +------------------------------------------------------+
            // | Sink                                                 |
            // +------------------------------------------------------+                 
            
                snk_pots_cfg_destroy(cfgs->snk_pots_ssl_config);

        // +----------------------------------------------------------+
        // | SST                                                      |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Module                                               |
            // +------------------------------------------------------+  

                mod_sst_cfg_destroy(cfgs->mod_sst_config);
                
            // +------------------------------------------------------+
            // | Message                                              |
            // +------------------------------------------------------+  

                msg_tracks_cfg_destroy(cfgs->msg_tracks_sst_config);
                            
            // +------------------------------------------------------+
            // | Sink                                                 |
            // +------------------------------------------------------+                 
            
                snk_tracks_cfg_destroy(cfgs->snk_tracks_sst_config);
/*
        // +----------------------------------------------------------+
        // | SSS                                                      |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Module                                               |
            // +------------------------------------------------------+  

                mod_sss_cfg_destroy(cfgs->mod_sss_config);
                
            // +------------------------------------------------------+
            // | Message                                              |
            // +------------------------------------------------------+  

                msg_spectra_cfg_destroy(cfgs->msg_spectra_sss_seps_config);
                msg_spectra_cfg_destroy(cfgs->msg_spectra_sss_pfs_config);

        // +----------------------------------------------------------+
        // | ISTFT                                                    |
        // +----------------------------------------------------------+

            // +------------------------------------------------------+
            // | Module                                               |
            // +------------------------------------------------------+

                mod_istft_cfg_destroy(cfgs->mod_istft_seps_config);
                mod_istft_cfg_destroy(cfgs->mod_istft_pfs_config);

            // +------------------------------------------------------+
            // | Message                                              |
            // +------------------------------------------------------+

                msg_hops_cfg_destroy(cfgs->msg_hops_seps_config);
                msg_hops_cfg_destroy(cfgs->msg_hops_pfs_config);

        // +----------------------------------------------------------+
        // | Resample                                                 |
        // +----------------------------------------------------------+

            // +------------------------------------------------------+
            // | Module                                               |
            // +------------------------------------------------------+

                mod_resample_cfg_destroy(cfgs->mod_resample_seps_config);
                mod_resample_cfg_destroy(cfgs->mod_resample_pfs_config);

            // +------------------------------------------------------+
            // | Message                                              |
            // +------------------------------------------------------+

                msg_hops_cfg_destroy(cfgs->msg_hops_seps_rs_config);
                msg_hops_cfg_destroy(cfgs->msg_hops_pfs_rs_config);

            // +------------------------------------------------------+
            // | Sink                                                 |
            // +------------------------------------------------------+

                snk_hops_cfg_destroy(cfgs->snk_hops_seps_rs_config);
                snk_hops_cfg_destroy(cfgs->snk_hops_pfs_rs_config);
*/
        free((void *) cfgs);

    }
