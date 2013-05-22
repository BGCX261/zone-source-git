digraph quota_v2_c{
        subgraph cluster_v2_check_quota_file{
                label="static int v2_check_quota_file(struct super_block *sb, int type)
";
                set_quota_magics_V2_INITQMAGICS[label="set_quota_magics_V2_INITQMAGICS"];
                set_quota_versions_V2_INITQVERSIONS[label="set_quota_versions_V2_INITQVERSIONS"];
                set_size_sb_s_op_quota_read_sb_type_dqhead_sizeof_struct_v2_disk_dqheader_0[label="set_size_sb_s_op_quota_read_sb_type_dqhead_sizeof_struct_v2_disk_dqheader_0"];
                size_not_eq_sizeof_struct_v2_disk_dqheader[label="size_not_eq_sizeof_struct_v2_disk_dqheader"];
                return_0[label="return_0"];
                le32_to_cpu_dqhead_dqh_magic_not_eq_quota_magics_type_or_le32_to_cpu_dqhead_dqh_version_not_eq_quota_versions_type[label="le32_to_cpu_dqhead_dqh_magic_not_eq_quota_magics_type_or_le32_to_cpu_dqhead_dqh_version_not_eq_quota_versions_type"];
                return_0[label="return_0"];
                return_1[label="return_1"];
        }

        subgraph cluster_v2_read_file_info{
                label="static int v2_read_file_info(struct super_block *sb, int type)
";
                set_info_sb_dqopt_sb_info_add_type[label="set_info_sb_dqopt_sb_info_add_type"];
                set_size_sb_s_op_quota_read_sb_type_dinfo_sizeof_struct_v2_disk_dqinfo_V2_DQINFOOFF[label="set_size_sb_s_op_quota_read_sb_type_dinfo_sizeof_struct_v2_disk_dqinfo_V2_DQINFOOFF"];
                size_not_eq_sizeof_struct_v2_disk_dqinfo[label="size_not_eq_sizeof_struct_v2_disk_dqinfo"];
                return_neg1[label="return_neg1"];
                set_info_dqi_brgrace_le32_to_cpu_dinfo_dqi_bgrace[label="set_info_dqi_brgrace_le32_to_cpu_dinfo_dqi_bgrace"];
                set_info_dqi_igrace_le32_to_cpu_dinfo_dqi_igrace[label="set_info_dqi_igrace_le32_to_cpu_dinfo_dqi_igrace"];
                set_info_dqi_flags_le32_to_cpu_dinfo_dqi_flags[label="set_info_dqi_flags_le32_to_cpu_dinfo_dqi_flags"];
                set_info_u_v2_i_dqi_blocks_le32_to_cpu_dinfo_dqi_blocks[label="set_info_u_v2_i_dqi_blocks_le32_to_cpu_dinfo_dqi_blocks"];
                set_info_u_v2_i_dqi_free_blk_le32_to_cpu_dinfo_dqi_free_blk[label="set_info_u_v2_i_dqi_free_blk_le32_to_cpu_dinfo_dqi_free_blk"];
                set_info_u_v2_i_dqi_free_entry_le32_to_cpu_dinfo_dqi_free_entry[label="set_info_u_v2_i_dqi_free_entry_le32_to_cpu_dinfo_dqi_free_entry"];
                return_0[label="return_0"];
        }

        subgraph cluster_v2_write_file_info{
                label="static int v2_write_file_info(struct super_block *sb, int type)
";
                set_info_sb_dqopt_sb_info_add_type[label="set_info_sb_dqopt_sb_info_add_type"];
                lock_dq_data_lock[label="lock_dq_data_lock"];
                set_info_dqi_flags_clear_DQF_INFO_DIRTY[label="set_info_dqi_flags_clear_DQF_INFO_DIRTY"];
                set_dinfo_dqi_bgrace_cpu_to_le32_info_dqi_bgrace[label="set_dinfo_dqi_bgrace_cpu_to_le32_info_dqi_bgrace"];
                set_dinfo_dqi_igrace_cpu_to_le32_info_dqi_igrace[label="set_dinfo_dqi_igrace_cpu_to_le32_info_dqi_igrace"];
                set_dinfo_dqi_flags_cpu_to_le32_info_dqi_flags_DQF_MASK_set[label="set_dinfo_dqi_flags_cpu_to_le32_info_dqi_flags_DQF_MASK_set"];
                unlock_dq_data_lock[label="unlock_dq_data_lock"];
                set_dinfo_dqi_blocks_cpu_to_le32_info_u_v2_i_dqi_blocks[label="set_dinfo_dqi_blocks_cpu_to_le32_info_u_v2_i_dqi_blocks"];
                set_dinfo_dqi_free_blk_cpu_to_le32_info_u_v2_i_dqi_free_entry[label="set_dinfo_dqi_free_blk_cpu_to_le32_info_u_v2_i_dqi_free_entry"];
                set_size_sb_s_op_quota_write_sb_type_dinfo_sizeof_struct_v2_disk_dqinfo_V2_DQINFOOFF[label="set_size_sb_s_op_quota_write_sb_type_dinfo_sizeof_struct_v2_disk_dqinfo_V2_DQINFOOFF"];
                size_not_eq_sizeof_v2_disk_dqinfo[label="size_not_eq_sizeof_v2_disk_dqinfo"];
                return_neg1[label="return_neg1"];
                return_0[label="return_0"];
        }

        subgraph cluster_disk2memdqb{
                label="static void disk2memdqb(struct mem_dqblk *m, struct v2_disk_dqblk *d)
";
                set_m_dqb_ihardlimit_le32_to_cpu_d_dqb_ihardlimit[label="set_m_dqb_ihardlimit_le32_to_cpu_d_dqb_ihardlimit"];
                set_m_dqb_isoftlimit_le32_to_cpu_d_dqb_isoftlimit[label="set_m_dqb_isoftlimit_le32_to_cpu_d_dqb_isoftlimit"];
                set_m_dqb_curinodes_le32_to_cpu_d_dqb_curinodes[label="set_m_dqb_curinodes_le32_to_cpu_d_dqb_curinodes"];
                set_m_dqb_itime_le64_to_cpu_d_dqb_bhardlimit[label="set_m_dqb_itime_le64_to_cpu_d_dqb_bhardlimit"];
                set_m_dqb_bhardlimit_le32_to_cpu_d_dqb_bhardlimit[label="set_m_dqb_bhardlimit_le32_to_cpu_d_dqb_bhardlimit"];
                set_m_dqb_bsoftlimit_le32_to_cpu_d_dqb_bsoftlimit[label="set_m_dqb_bsoftlimit_le32_to_cpu_d_dqb_bsoftlimit"];
                set_m_dqb_curspace_le64_to_cpu_d_dqb_curspace[label="set_m_dqb_curspace_le64_to_cpu_d_dqb_curspace"];
                set_m_dqb_btime_le64_to_cpu_d_dqb_btime[label="set_m_dqb_btime_le64_to_cpu_d_dqb_btime"];
        }

        subgraph cluster_mem2diskdqb{
                label="static void mem2diskdqb(struct v2_disk_dqblk *d, struct mem_dqblk *m, qid_t id)
";
                set_d_dpb_ihardlimit_cpu_to_le32_m_dqb_ihardlimit[label="set_d_dpb_ihardlimit_cpu_to_le32_m_dqb_ihardlimit"];
                set_d_dqb_isoftlimit_cpu_to_le32_m_dqb_isoftlimit[label="set_d_dqb_isoftlimit_cpu_to_le32_m_dqb_isoftlimit"];
                set_d_dqb_curinodes_cpu_to_le32_m_dqb_curinodes[label="set_d_dqb_curinodes_cpu_to_le32_m_dqb_curinodes"];
                set_m_dqb_itime_cpu_to_le64_d_dqb_bhardlimit[label="set_m_dqb_itime_cpu_to_le64_d_dqb_bhardlimit"];
                set_m_dqb_bhardlimit_cpu_to_le32_d_dqb_bhardlimit[label="set_m_dqb_bhardlimit_cpu_to_le32_d_dqb_bhardlimit"];
                set_m_dqb_bsoftlimit_cpu_to_le32_d_dqb_bsoftlimit[label="set_m_dqb_bsoftlimit_cpu_to_le32_d_dqb_bsoftlimit"];
                set_m_dqb_curspace_cpu_to_le64_d_dqb_curspace[label="set_m_dqb_curspace_cpu_to_le64_d_dqb_curspace"];
                set_m_dqb_btime_cpu_to_le64_d_dqb_btime[label="set_m_dqb_btime_cpu_to_le64_d_dqb_btime"];
                set_d_dqb_id_cpu_to_le32_id[label="set_d_dqb_id_cpu_to_le32_id"];
        }

        subgraph cluster_getdqbuf{
                label="static dqbuf_t getdqbuf(void)
";
                set_buf_kmalloc_V2_DQBLKSIZE[label="set_buf_kmalloc_V2_DQBLKSIZE"];
                return_buf[label="return_buf"];
        }

        subgraph cluster_freedqbuf{
                label="static inline void freedqbuf(dqbuf_t buf)
";
                kfree_buf[label="kfree_buf"];
        }

        subgraph cluster_read_blk{
                label="static inline ssize_t read_blk(struct super_block *sb, int type, uint blk, dqbuf_t buf)
";
                memset_buf_0[label="memset_buf_0"];
                return_sb_s_op_quota_read_sb_type_buf_V2_DQBLKSIZE_blk_left_move_V2_DQBLKSIZE_BITS[label="return_sb_s_op_quota_read_sb_type_buf_V2_DQBLKSIZE_blk_left_move_V2_DQBLKSIZE_BITS"];
        }

        subgraph cluster_write_blk{
                label="static inline ssize_t write_blk(struct super_block *sb, int type, uint blk, dqbuf_t buf)
";
                return_sb_s_op_quota_write_sb_type_buf_V2_DQBLKSIZE_blk_left_move_V2_DQBLKSIZE_BITS[label="return_sb_s_op_quota_write_sb_type_buf_V2_DQBLKSIZE_blk_left_move_V2_DQBLKSIZE_BITS"];
        }

        subgraph cluster_get_free_dqblk{
                label="static int get_free_dqblk(struct super_block *sb, int type)
";
                set_buf_getdqbuf[label="set_buf_getdqbuf"];
                set_info_sb_dqinfo_sb_type[label="set_info_sb_dqinfo_sb_type"];
                set_dh_buf[label="set_dh_buf"];
                info_u_v2_i_dqi_free_blk_true[label="info_u_v2_i_dqi_free_blk_true"];
                set_blk_info_u_v2_i_dqi_free_blk[label="set_blk_info_u_v2_i_dqi_free_blk"];
                set_ret_read_blk_sb_type_blk_buf_small_0[label="set_ret_read_blk_sb_type_blk_buf_small_0"];
                goto_out_buf[label="goto_out_buf"];
                set_info_u_v2_i_dqi_free_blk_le32_to_cpu_dh_dqdh_next_free[label="set_info_u_v2_i_dqi_free_blk_le32_to_cpu_dh_dqdh_next_free"];
                memset_buf_0[label="memset_buf_0"];
                set_ret_write_blk_sb_type_info_u_v2_i_dqi_blocks_buf_true[label="set_ret_write_blk_sb_type_info_u_v2_i_dqi_blocks_buf_true"];
                goto_out_buf[label="goto_out_buf"];
                inc_blk_info_u_v2_i_dqi_blocks[label="inc_blk_info_u_v2_i_dqi_blocks"];
                mark_info_dirty_sb_type[label="mark_info_dirty_sb_type"];
                set_ret_blk[label="set_ret_blk"];
                freedqbuf_buf[label="freedqbuf_buf"];
                return_ret[label="return_ret"];
        }

        subgraph cluster_put_free_dqblk{
                label="static int put_free_dqblk(struct super_block *sb, int type, dqbuf_t buf, uint blk)
";
                set_info_sb_dqinfo_sb_type[label="set_info_sb_dqinfo_sb_type"];
                set_dh_buf[label="set_dh_buf"];
                set_dh_dqdh_next_free_cpu_to_le32_info_u_v2_i_dqi_free_blk[label="set_dh_dqdh_next_free_cpu_to_le32_info_u_v2_i_dqi_free_blk"];
                set_dh_dqdh_prev_free_cpu_to_le32_0[label="set_dh_dqdh_prev_free_cpu_to_le32_0"];
                set_dh_dqdh_entries_cpu_to_le64_0[label="set_dh_dqdh_entries_cpu_to_le64_0"];
                set_info_u_v2_i_dqi_free_blk_blk[label="set_info_u_v2_i_dqi_free_blk_blk"];
                mark_info_dirty_sb_type[label="mark_info_dirty_sb_type"];
                set_err_write_blk_sb_type_blk_buf_small_0[label="set_err_write_blk_sb_type_blk_buf_small_0"];
                return_err[label="return_err"];
                return_0[label="return_0"];
        }

        subgraph cluster_remove_free_dqentry{
                label="static int remove_free_dqentry(struct super_block *sb, int type, dqbuf_t buf, uint blk)
";
                set_tmpbuf_getdqbuf[label="set_tmpbuf_getdqbuf"];
                set_info_sb_dqinfo_sb_type[label="set_info_sb_dqinfo_sb_type"];
                set_dh_buf[label="set_dh_buf"];
                set_nextblk_le32_to_cpu_dh_dqdh_next_free[label="set_nextblk_le32_to_cpu_dh_dqdh_next_free"];
                set_prev_blk_le32_to_cpu_dh_dqdh_prev_free[label="set_prev_blk_le32_to_cpu_dh_dqdh_prev_free"];
                nextblk_true[label="nextblk_true"];
                set_err_read_blk_sb_type_nextblk_tmpbuf_small_0[label="set_err_read_blk_sb_type_nextblk_tmpbuf_small_0"];
                goto_out_buf[label="goto_out_buf"];
                set_tmpbuf_dqdh_prev_free_dh_dqdh_prev_free[label="set_tmpbuf_dqdh_prev_free_dh_dqdh_prev_free"];
                set_err_write_blk_sb_type_nextblk_tmpbuf_small_0[label="set_err_write_blk_sb_type_nextblk_tmpbuf_small_0"];
                goto_out_buf[label="goto_out_buf"];
                prevblk_true[label="prevblk_true"];
                set_err_read_blk_sb_type_prevblk_tmpbuf_small_0[label="set_err_read_blk_sb_type_prevblk_tmpbuf_small_0"];
                goto_out_buf[label="goto_out_buf"];
                set_tmpbuf_dqdh_next_free_dh_dqdh_next_free[label="set_tmpbuf_dqdh_next_free_dh_dqdh_next_free"];
                set_err_write_blk_sb_type_prevblk_tmpbuf_small_0[label="set_err_write_blk_sb_type_prevblk_tmpbuf_small_0"];
                goto_out_buf[label="goto_out_buf"];
                set_info_u_v2_i_dqi_free_entry_nextblk[label="set_info_u_v2_i_dqi_free_entry_nextblk"];
                mark_info_dirty_sb_type[label="mark_info_dirty_sb_type"];
                freedqbuf_tmpbuf[label="freedqbuf_tmpbuf"];
                set_dh_dqdh_next_free_dh_dqdh_prev_free_cpu_to_le32_0[label="set_dh_dqdh_next_free_dh_dqdh_prev_free_cpu_to_le32_0"];
                write_blk_sb_type_blk_buf_small_0[label="write_blk_sb_type_blk_buf_small_0"];
                return_0[label="return_0"];
                freedqbuf_tmpbuf[label="freedqbuf_tmpbuf"];
                return_err[label="return_err"];
        }

        subgraph cluster_insert_free_dqentry{
                label="static int insert_free_dqentry(struct super_block *sb, int type, dqbuf_t buf, uint blk)
";
                set_tmpbuf_getdqbuf[label="set_tmpbuf_getdqbuf"];
                set_info_sb_dqinfo_sb_type[label="set_info_sb_dqinfo_sb_type"];
                set_dh_buf[label="set_dh_buf"];
                set_dh_dqdh_next_free_cpu_to_le32_info_u_v2_dqi_free_entry[label="set_dh_dqdh_next_free_cpu_to_le32_info_u_v2_dqi_free_entry"];
                set_dh_dqdh_prev_free_cpu_to_le32_0[label="set_dh_dqdh_prev_free_cpu_to_le32_0"];
                set_err_write_blk_sb_type_blk_buf_small_0[label="set_err_write_blk_sb_type_blk_buf_small_0"];
                goto_out_buf[label="goto_out_buf"];
                info_u_v2_i_dqi_free_entry_true[label="info_u_v2_i_dqi_free_entry_true"];
                set_err_read_blk_sb_type_info_u_v2_i_dqi_free_entry_tmp_buf_small_0[label="set_err_read_blk_sb_type_info_u_v2_i_dqi_free_entry_tmp_buf_small_0"];
                goto_out_buf[label="goto_out_buf"];
                set_tmpbuf_dqdh_prev_free_cpu_to_le32_blk[label="set_tmpbuf_dqdh_prev_free_cpu_to_le32_blk"];
                set_err_write_blk_sb_type_info_u_v2_i_dqi_free_entry_tmpbuf_small_0[label="set_err_write_blk_sb_type_info_u_v2_i_dqi_free_entry_tmpbuf_small_0"];
                goto_out_buf[label="goto_out_buf"];
                freedqbuf_tmpbuf[label="freedqbuf_tmpbuf"];
                set_info_u_v2_i_dqi_free_entry_blk[label="set_info_u_v2_i_dqi_free_entry_blk"];
                mark_info_dirty_sb_type[label="mark_info_dirty_sb_type"];
                return_0[label="return_0"];
                freedqbuf_tmpbuf[label="freedqbuf_tmpbuf"];
                return_err[label="return_err"];
        }

        subgraph cluster_find_free_dqentry{
                label="static uint find_free_dqentry(struct dquot *dquot, int *err)
";
                set_sb_dquot_dq_sb[label="set_sb_dquot_dq_sb"];
                set_info_sb_dqopt_sb_info_add_dquot_dq_type[label="set_info_sb_dqopt_sb_info_add_dquot_dq_type"];
                set_buf_getdqbuf_false[label="set_buf_getdqbuf_false"];
                set_error_ENOMEM[label="set_error_ENOMEM"];
                return_0[label="return_0"];
                set_dh_buf[label="set_dh_buf"];
                set_ddquot_GETENTRIES_buf[label="set_ddquot_GETENTRIES_buf"];
                info_u_v2_i_dqi_free_entry_true[label="info_u_v2_i_dqi_free_entry_true"];
                set_blk_info_u_v2_i_dqi_free_entry[label="set_blk_info_u_v2_i_dqi_free_entry"];
                set_err_read_blk_sb_dquot_dq_type_blk_buf_small_0[label="set_err_read_blk_sb_dquot_dq_type_blk_buf_small_0"];
                goto_out_buf[label="goto_out_buf"];
                set_blk_get_free_dqblk_sb_dquot_dq_type[label="set_blk_get_free_dqblk_sb_dquot_dq_type"];
                blk_small_0[label="blk_small_0"];
                set_err_blk[label="set_err_blk"];
                freedqbuf_buf[label="freedqbuf_buf"];
                return_0[label="return_0"];
                memset_buf_0[label="memset_buf_0"];
                set_info_u_v2_i_dqi_free_entry_blk[label="set_info_u_v2_i_dqi_free_entry_blk"];
                mark_info_dirty_sb_dquot_dq_type[label="mark_info_dirty_sb_dquot_dq_type"];
                memset_buf_0[label="memset_buf_0"];
                set_info_u_v2_i_dqi_free_entry_blk[label="set_info_u_v2_i_dqi_free_entry_blk"];
                mark_info_dirty_sb_dquot_dq_type[label="mark_info_dirty_sb_dquot_dq_type"];
                le16_to_cpu_dh_dqdh_entries_add_1_not_small_V2_DQSTRINBLK[label="le16_to_cpu_dh_dqdh_entries_add_1_not_small_V2_DQSTRINBLK"];
                set_err_remove_free_dqentry_sb_dquot_dq_type_buf_blk_small_0[label="set_err_remove_free_dqentry_sb_dquot_dq_type_buf_blk_small_0"];
                goto_out_buf[label="goto_out_buf"];
                set_dh_dqqdh_entries_cpu_to_le16_le16_to_cpu_dh_dqdh_entries_add_1[label="set_dh_dqqdh_entries_cpu_to_le16_le16_to_cpu_dh_dqdh_entries_add_1"];
                memset_fakedquot_0[label="memset_fakedquot_0"];
                for_i_V2_DQSTRINBLK_true_and_memcpy_fakedquot_ddquot[label="for_i_V2_DQSTRINBLK_true_and_memcpy_fakedquot_ddquot"];
                set_err_write_blk_sb_dquot_dq_type_blk_buf_small_0[label="set_err_write_blk_sb_dquot_dq_type_blk_buf_small_0"];
                goto_out_buf[label="goto_out_buf"];
                set_dquot_dq_off_blk_left_move_V2_DQBLKSIZE_BITS_add_sizeof_struct_v2_disk_dqdheader_add+i_multi_sizeof_struct_v2_disk_dqblk[label="set_dquot_dq_off_blk_left_move_V2_DQBLKSIZE_BITS_add_sizeof_struct_v2_disk_dqdheader_add+i_multi_sizeof_struct_v2_disk_dqblk"];
                freedqbuf_buf[label="freedqbuf_buf"];
                return_blk[label="return_blk"];
                freedqbuf_buf[label="freedqbuf_buf"];
                return_0[label="return_0"];
        }

        subgraph cluster_do_insert_tree{
                label="static int do_insert_tree(struct dquot *dquot, uint *treeblk, int depth)
";
                set_sb_dquot_dq_sb[label="set_sb_dquot_dq_sb"];
                set_buf_getdqbuf_false[label="set_buf_getdqbuf_false"];
                return_ENOMEM[label="return_ENOMEM"];
                treeblk_false[label="treeblk_false"];
                set_ret_get_free_dqblk_sb_dquot_dq_type[label="set_ret_get_free_dqblk_sb_dquot_dq_type"];
                ret_small_0[label="ret_small_0"];
                goto_out_buf[label="goto_out_buf"];
                set_treeblk_ret[label="set_treeblk_ret"];
                memset_buf_0[label="memset_buf_0"];
                set_newact_1[label="set_newact_1"];
                set_ret_read_blk_sb_dquot_dq_type_treeblk_buf_small_0[label="set_ret_read_blk_sb_dquot_dq_type_treeblk_buf_small_0"];
                goto_out_buf[label="goto_out_buf"];
                set_ref_buf[label="set_ref_buf"];
                set_newblk_le32_to_cpu_ref_GETIDINDEX_dquot_dq_id_depth[label="set_newblk_le32_to_cpu_ref_GETIDINDEX_dquot_dq_id_depth"];
                newblk_false[label="newblk_false"];
                set_newson_1[label="set_newson_1"];
                depth_eq_V2_DQTREEDEPTH_sub_1[label="depth_eq_V2_DQTREEDEPTH_sub_1"];
                set_newblk_find_free_dentry_dquot_ret[label="set_newblk_find_free_dentry_dquot_ret"];
                set_ret_do_insert_tree_dquot_newblk_depth_add_1[label="set_ret_do_insert_tree_dquot_newblk_depth_add_1"];
                newson_true_and_ret_not_small_0[label="newson_true_and_ret_not_small_0"];
                set_ref_GETIDINDEX_dquot_dq_id_depth_cpu_to_le32_newblk[label="set_ref_GETIDINDEX_dquot_dq_id_depth_cpu_to_le32_newblk"];
                set_ret_write_blk_sb_dquot_dq_type_treeblk_buf[label="set_ret_write_blk_sb_dquot_dq_type_treeblk_buf"];
                newact_true_and_ret_small_0[label="newact_true_and_ret_small_0"];
                put_free_dqblk_sb_dquot_dq_type_buf_treeblk[label="put_free_dqblk_sb_dquot_dq_type_buf_treeblk"];
                freedqbuf_buf[label="freedqbuf_buf"];
                return_ret[label="return_ret"];
        }

        subgraph cluster_dq_insert_tree{
                label="static inline int dq_insert_tree(struct dquot *dquot)
";
                set_tmp_V2_DQTREEOFF[label="set_tmp_V2_DQTREEOFF"];
                return_do_insert_tree_dquot_tmp_0[label="return_do_insert_tree_dquot_tmp_0"];
        }

        subgraph cluster_v2_write_dquot{
                label="static int v2_write_dquot(struct dquot *dquot)
";
                set_type_dquot_dq_type[label="set_type_dquot_dq_type"];
                dquot_dq_off_false[label="dquot_dq_off_false"];
                set_ret_dq_insert_tree_dquot_small_0[label="set_ret_dq_insert_tree_dquot_small_0"];
                return_ret[label="return_ret"];
                lock_dq_data_lock[label="lock_dq_data_lock"];
                mem2diskdqb_ddquot_dquot_dq_dqb_dquot_dq_id[label="mem2diskdqb_ddquot_dquot_dq_dqb_dquot_dq_id"];
                memset_empty_0[label="memset_empty_0"];
                memcpy_empty_ddquot_false[label="memcpy_empty_ddquot_false"];
                set_ddquot_dqd_itime_cpu_to_le64_1[label="set_ddquot_dqd_itime_cpu_to_le64_1"];
                unlock_dq_data_lock[label="unlock_dq_data_lock"];
                set_ret_dquot_dq_sb_s_op_quota_write_dquot_dq_sb_type_ddquot_sizeof_struct_v2_disk_dqblk_dquot_dq_off[label="set_ret_dquot_dq_sb_s_op_quota_write_dquot_dq_sb_type_ddquot_sizeof_struct_v2_disk_dqblk_dquot_dq_off"];
                ret_not_eq_sizeof_struct_v2_disk_dqblk[label="ret_not_eq_sizeof_struct_v2_disk_dqblk"];
                ret_not_small_0[label="ret_not_small_0"];
                set_ret_ENOSPC[label="set_ret_ENOSPC"];
                set_ret_0[label="set_ret_0"];
                inc_dqstats_writes[label="inc_dqstats_writes"];
                return_ret[label="return_ret"];
        }

        subgraph cluster_free_dqentry{
                label="static int free_dqentry(struct dquot *dquot, uint blk)
";
                set_sb_dquot_dq_sb[label="set_sb_dquot_dq_sb"];
                set_type_dquot_dq_type[label="set_type_dquot_dq_type"];
                set_buf_getdqbuf[label="set_buf_getdqbuf"];
                dquot_dq_off_right_move_V2_DQBLKSIZE_BITS_not_eq_blk[label="dquot_dq_off_right_move_V2_DQBLKSIZE_BITS_not_eq_blk"];
                goto_out_buf[label="goto_out_buf"];
                set_ret_read_blk_sb_type_blk_buf_small_0[label="set_ret_read_blk_sb_type_blk_buf_small_0"];
                goto_out_buf[label="goto_out_buf"];
                set_dh_struct_v2_disk_dqdbheader_buf[label="set_dh_struct_v2_disk_dqdbheader_buf"];
                set_dh_dqdh_entries_cpu_to_le16_le16_to_cpu_dh_dqdh_entries_sub_1[label="set_dh_dqdh_entries_cpu_to_le16_le16_to_cpu_dh_dqdh_entries_sub_1"];
                le16_to_cpu_dh_dqdh_entries_false[label="le16_to_cpu_dh_dqdh_entries_false"];
                set_ret_remove_free_dqentry_sb_type_buf_blk_small_0_or_set_ret_put_free_dqblk_sb_type_buf_blk_small_0[label="set_ret_remove_free_dqentry_sb_type_buf_blk_small_0_or_set_ret_put_free_dqblk_sb_type_buf_blk_small_0"];
                goto_out_buf[label="goto_out_buf"];
                memset_buf_add_dquot_dq_off_and_1_left_move_V2_DQBLKSIZE_BITS_sub_1_0_sizeof_struct_v2_disk_dqblk[label="memset_buf_add_dquot_dq_off_and_1_left_move_V2_DQBLKSIZE_BITS_sub_1_0_sizeof_struct_v2_disk_dqblk"];
                le16_to_cpu_dh_dqdh_entries_eq_V2_DQSTRINBLK_sub_1[label="le16_to_cpu_dh_dqdh_entries_eq_V2_DQSTRINBLK_sub_1"];
                set_ret_insert_free_dqentry_sb_type_buf_blk_small_0[label="set_ret_insert_free_dqentry_sb_type_buf_blk_small_0"];
                goto_out_buf[label="goto_out_buf"];
                set_ret_write_blk_sb_type_blk_buf_small_0[label="set_ret_write_blk_sb_type_blk_buf_small_0"];
                goto_out_buf[label="goto_out_buf"];
                set_dquot_dq_off_0[label="set_dquot_dq_off_0"];
                freedqbuf_buf[label="freedqbuf_buf"];
                return_ret[label="return_ret"];
        }

        subgraph cluster_remove_tree{
                label="static int remove_tree(struct dquot *dquot, uint *blk, int depth)
";
                set_sb_dquot_dq_sb[label="set_sb_dquot_dq_sb"];
                set_type_dquot_dq_type[label="set_type_dquot_dq_type"];
                set_buf_getdqbuf[label="set_buf_getdqbuf"];
                set_ref_buf[label="set_ref_buf"];
                set_ret_read_blk_sb_type_blk_buf_small_0[label="set_ret_read_blk_sb_type_blk_buf_small_0"];
                goto_out_buf[label="goto_out_buf"];
                set_newblk_le32_to_cpu_ref_GETIDINDEX_dquot_dq_id_depth[label="set_newblk_le32_to_cpu_ref_GETIDINDEX_dquot_dq_id_depth"];
                depth_eq_V2_DQTREEDEPTH_sub_1[label="depth_eq_V2_DQTREEDEPTH_sub_1"];
                set_ret_free_dqentry_dquot_newblk[label="set_ret_free_dqentry_dquot_newblk"];
                set_newblk_0[label="set_newblk_0"];
                set_ret_remove_tree_dquot_newblk_depth_add_1[label="set_ret_remove_tree_dquot_newblk_depth_add_1"];
                ret_not_small_0_and_newblk_true[label="ret_not_small_0_and_newblk_true"];
                set_ref_GETIDINDEX_dquot_dq_id_depth_cpu_to_le32_0[label="set_ref_GETIDINDEX_dquot_dq_id_depth_cpu_to_le32_0"];
                for_i_V2_DQBLKSIZE_and_buf_i_false[label="for_i_V2_DQBLKSIZE_and_buf_i_false"];
                i_eq_V2_DQBLKSIZE_and_blk_not_eq_V2_DQTREEOFF[label="i_eq_V2_DQBLKSIZE_and_blk_not_eq_V2_DQTREEOFF"];
                put_free_dqblk_sb_type_buf_blk[label="put_free_dqblk_sb_type_buf_blk"];
                set_blk_0[label="set_blk_0"];
                set_ret_write_blk_sb_type_blk_buf_small_0[label="set_ret_write_blk_sb_type_blk_buf_small_0"];
                freedqbuf_buf[label="freedqbuf_buf"];
                return_ret[label="return_ret"];
        }

        subgraph cluster_v2_delete_dquot{
                label="static int v2_delete_dquot(struct dquot *dquot)
";
                set_tmp_V2_DQTREEOFF[label="set_tmp_V2_DQTREEOFF"];
                dquot_dq_off_false[label="dquot_dq_off_false"];
                return_0[label="return_0"];
                return_remove_tree_dquot_tmp_0[label="return_remove_tree_dquot_tmp_0"];
        }

        subgraph cluster_find_block_dqentry{
                label="static loff_t find_block_dqentry(struct dquot *dquot, uint blk)
";
                set_buf_getdqbuf[label="set_buf_getdqbuf"];
                set_ddquot_GETENTRIES_buf[label="set_ddquot_GETENTRIES_buf"];
                set_ret_read_blk_dquot_dq_sb_dquot_dq_type_blk_buf_small_[label="set_ret_read_blk_dquot_dq_sb_dquot_dq_type_blk_buf_small_"];
                goto_out_buf[label="goto_out_buf"];
                dquot_dq_id_true[label="dquot_dq_id_true"];
                for_i_V2_DQSTRINBLK_and_le32_to_cpu_ddquot_i_dqb_id_not_eq_dquot_dq_id[label="for_i_V2_DQSTRINBLK_and_le32_to_cpu_ddquot_i_dqb_id_not_eq_dquot_dq_id"];
                memset_fakedquot_0[label="memset_fakedquot_0"];
                for_i_V2_DQSTRINBLK[label="for_i_V2_DQSTRINBLK"];
                le32_to_cpu_ddquot_i_dqb_id_false_and_memcmp_fakedquot_ddquot_add_i_sizeof_struct_v2_disk_dqblk[label="le32_to_cpu_ddquot_i_dqb_id_false_and_memcmp_fakedquot_ddquot_add_i_sizeof_struct_v2_disk_dqblk"];
                i_eq_V2_DQSTRINBLK[label="i_eq_V2_DQSTRINBLK"];
                set_ret_EIO[label="set_ret_EIO"];
                goto_out_buf[label="goto_out_buf"];
                set_ret_blk_left_move_V2_DQBLKSIZE_BITS_add_sizeof_struct_v2_disk_dqdbheader_add_i_multi_sizeof_struct_v2_disk_dqblk[label="set_ret_blk_left_move_V2_DQBLKSIZE_BITS_add_sizeof_struct_v2_disk_dqdbheader_add_i_multi_sizeof_struct_v2_disk_dqblk"];
                freedqbuf_buf[label="freedqbuf_buf"];
                return_ret[label="return_ret"];
        }

        subgraph cluster_find_tree_dqentry{
                label="static loff_t find_tree_dqentry(struct dquot *dquot, uint blk, int depth)
";
                set_buf_getbuf[label="set_buf_getbuf"];
                set_ref_buf[label="set_ref_buf"];
                set_ret_read_blk_dquot_dq_sb_dquot_dq_type_blk_buf_small_0[label="set_ret_read_blk_dquot_dq_sb_dquot_dq_type_blk_buf_small_0"];
                goto_out_buf[label="goto_out_buf"];
                set_ret_0[label="set_ret_0"];
                set_blk_le32_to_cpu_ref_GETIDINDEX_dquot_dq_id_depth[label="set_blk_le32_to_cpu_ref_GETIDINDEX_dquot_dq_id_depth"];
                blk_false[label="blk_false"];
                goto_out_buf[label="goto_out_buf"];
                depth_small_V2_DQTREEDEPTH_sub_1[label="depth_small_V2_DQTREEDEPTH_sub_1"];
                set_ret_find_tree_dqentry_dquot_blk_depth_add_1[label="set_ret_find_tree_dqentry_dquot_blk_depth_add_1"];
                set_ret_find_block_dqentry_dquot_blk[label="set_ret_find_block_dqentry_dquot_blk"];
                freedqbuf_buf[label="freedqbuf_buf"];
                return_ret[label="return_ret"];
        }

        subgraph cluster_find_dqentry{
                label="static inline loff_t find_dqentry(struct dquot *dquot)
";
                return_find_tree_dqentry_dquot_V2_DQTREEOFF_0[label="return_find_tree_dqentry_dquot_V2_DQTREEOFF_0"];
        }

        subgraph cluster_v2_read_dquot{
                label="static int v2_read_dquot(struct dquot *dquot)
";
                set_type_dquot_dq_type[label="set_type_dquot_dq_type"];
                set_offset_find_dqentry_dquot[label="set_offset_find_dqentry_dquot"];
                offset_not_great_0[label="offset_not_great_0"];
                offset_small_0[label="offset_small_0"];
                set_dquot_dq_off_0[label="set_dquot_dq_off_0"];
                set_bit_DQ_FAKE_B_dquot_dq_flags[label="set_bit_DQ_FAKE_B_dquot_dq_flags"];
                memset_dquot_dq_dqb_0[label="memset_dquot_dq_dqb_0"];
                set_ret_offset[label="set_ret_offset"];
                set_dquot_dq_off_offset[label="set_dquot_dq_off_offset"];
                set_ret_dquot_dq_sb_s_op_quota_read_dquot_dq_sb_type_ddquot_sizeof_struct_v2_disk_dqblk_offset_not_eq_sizeof_struct_v2_disk_dqblk[label="set_ret_dquot_dq_sb_s_op_quota_read_dquot_dq_sb_type_ddquot_sizeof_struct_v2_disk_dqblk_offset_not_eq_sizeof_struct_v2_disk_dqblk"];
                ret_not_small_0[label="ret_not_small_0"];
                set_ret_EIO[label="set_ret_EIO"];
                memset_ddquot_0[label="memset_ddquot_0"];
                set_ret_0[label="set_ret_0"];
                memset_empty_0[label="memset_empty_0"];
                set_empty_dqqb_itime_cpu_to_le64_1[label="set_empty_dqqb_itime_cpu_to_le64_1"];
                memcmp_empty_ddquot_false[label="memcmp_empty_ddquot_false"];
                set_ddquot_dqb_itime_0[label="set_ddquot_dqb_itime_0"];
                disk2memdqb_dquot_dq_dqb_ddquot[label="disk2memdqb_dquot_dq_dqb_ddquot"];
                dquot_dq_dqb_dqb_bhardlimit_false_and_dquot_dq_dqb_dqb_bsoftlimit_false_and_dquot_dq_dqb_dqb_ihardlimit_and_dquot_dq_dqb_dqb_isoftlimit_false[label="dquot_dq_dqb_dqb_bhardlimit_false_and_dquot_dq_dqb_dqb_bsoftlimit_false_and_dquot_dq_dqb_dqb_ihardlimit_and_dquot_dq_dqb_dqb_isoftlimit_false"];
                set_bit_DQ_FAKE_B_dquot_dq_flags[label="set_bit_DQ_FAKE_B_dquot_dq_flags"];
                inc_dqstats_reads[label="inc_dqstats_reads"];
                return_ret[label="return_ret"];
        }

        subgraph cluster_v2_release_dquot{
                label="static int v2_release_dquot(struct dquot *dquot)
";
                test_bit_DQ_FAKE_B_dquot_dq_flags_true_and_dquot_dq_dqb_dqb_curinodes_or_dquot_dq_dqb_dqb_curspace_false[label="test_bit_DQ_FAKE_B_dquot_dq_flags_true_and_dquot_dq_dqb_dqb_curinodes_or_dquot_dq_dqb_dqb_curspace_false"];
                return_v2_delete_dquot_dquot[label="return_v2_delete_dquot_dquot"];
                return_0[label="return_0"];
        }

        subgraph cluster_init_v2_quota_format{
                label="static int __init init_v2_quota_format(void)
";
                return_register_quota_format_v2_quota_format[label="return_register_quota_format_v2_quota_format"];
        }

        subgraph cluster_exit_v2_quota_format{
                label="static void __exit exit_v2_quota_format(void)
";
                unregister_quota_format__v2_quota_format[label="unregister_quota_format__v2_quota_format"];
        }
}
