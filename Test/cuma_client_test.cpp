#include "cuma_client_test.h"

Cuma_client_test::Cuma_client_test(QObject *parent)
{
    //파일의 주소를 현재 디렉토리로 설정함
    m_File->init_dir();

}

void Cuma_client_test::t_mf_command_set_file_name()
{

}

void Cuma_client_test::t_mf_command_set_unit_bypass_count()
{

}

void Cuma_client_test::t_mf_command_ping_test()
{

}

void Cuma_client_test::t_mf_command_spread_test()
{

}

void Cuma_client_test::t_mf_command_req_file_test()
{

}

void Cuma_client_test::t_mf_command_trace_pass_test()
{

}

int Cuma_client_test::env_set_test_env_unit()
{
    return 0;
}

void Cuma_client_test::env_clear_cache()
{
    m_Unit_delay_time_array.clear();
    m_Cuma_unit_list.clear();
    m_Cuma_unit_inside_timeout_unit_list.clear();
    m_Send_Unit_list.clear();
    m_File->clear_binary();
    m_active = true;
    m_limit_bypass_count = 0;
    m_file_info_block.clear();
    m_file_frag_address.clear();
}
