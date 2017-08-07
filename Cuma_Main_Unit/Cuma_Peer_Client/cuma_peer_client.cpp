#include "../cuma_main.h"

int Cuma_Main::mf_command_set_file_name(QString f_name)
{
    m_File->set_File_Name(f_name);
    return 0;
}

int Cuma_Main::mf_command_ping_test()
{
    try{
        
        //ping 테스트 하는 클라이언트
        Cuma_Debug("start_ping_test", __LINE__);

        //모든 유닛들을 ping으로 스크리밍해서 리스트 안에 넣음
        Cuma_Debug("scriming all unit and add list", __LINE__);
        if(m_Cuma_unit_list.count() == 0)
        {
            throw Cuma_Error("mf_command_ping_test: Cuma_unit_list is empty", __LINE__, m_Pid);
        }
        
        //모든 유닛들에게 전송함
        Cuma_Debug("send all unit ping protocol", __LINE__);

        for(QSharedPointer<Cuma_Main> p: m_Cuma_unit_list)
        {
            //만약 자기 유닛차례일 경우 continue
            if(p->mf_get_pid() == m_Pid)
            {
                continue;
            }

            client_send(cuma_protocol::req_ping_protocol(m_Pid), p);
        }

        return 0;
    }
    catch(Cuma_Error& e)
    {
        e.show_error_string();
        return -1;
    }
}

int Cuma_Main::mf_command_spread_test()
{
    try{
        
        //ping limit 내의 유닛들이 있는지
        Cuma_Debug("check unit have insie ping limit", __LINE__);
        if( ! (m_Cuma_unit_inside_timeout_unit_list.count() > 0))
        {
            throw Cuma_Error("mf_command_spread_test: no unit found in Cuma_unit_inside_timeout_list ", __LINE__, m_Pid);
        }
        
        //전송할 파일 바이너리를 읽음
        Cuma_Debug("read file binary to send", __LINE__);
        QByteArray spread_binary =  m_File->get_File_binary();
        
        //만약 spread_binary가 nullptr일경우 예외처리
        Cuma_Debug("exception when psread_binary is nullptr", __LINE__);
        if(spread_binary == nullptr)
        {
            throw Cuma_Error("mf_command_spread_test: Can't of read File bianry", __LINE__, m_Pid);
        }
        
        //전송할 파일들을 유닛수대로 파편화를 함
        Cuma_Debug("frag the file in insdie unit size", __LINE__);
        m_File->set_File_Frag_Count(m_Cuma_unit_inside_timeout_unit_list.count());

        //파일 frag를 만들기
        Cuma_Debug("making file frag...", __LINE__);
        if (m_File->make_frag() < 0)
        {
            throw Cuma_Error("mFile-> make_frag() error", __LINE__, m_Pid);
        }

        uint index = 0;
        
        //파일 유닛들의 send info 저장함
        QMap<uint32_t, QVector<uint32_t>> file_save_unit;
        
        //파일 파편화를 모든 유닛들에게 전송함
        Cuma_Debug("send all unit by file_fragmentation", __LINE__);
        foreach(QSharedPointer<Cuma_Main> p, m_Cuma_unit_inside_timeout_unit_list)
        {

            //파편화에 대한 json을 만듬
            Cuma_Debug("send file frag index :" + QString::number(p->mf_get_pid()), __LINE__);
            
            Cuma_Debug("send to unit Pid : " + QString::number(m_Pid), __LINE__);
            client_send(cuma_protocol::req_file_binary_save_protocol(m_File->get_File_Name(), index, m_File->get_File_Frag_By_Index(index), m_Pid ), p);
            
            Cuma_Debug("save send_frag_binary index : " + QString::number(index), __LINE__);
            QVector<uint32_t> file_Frag_index;
            file_Frag_index.append(p->mf_get_pid());
            
            file_save_unit[index] = file_Frag_index;
            
            index++;
        }
        
        //File_frag를 수신한 유닛들의 주소들을 frag인덱스별로 저장함
        Cuma_Debug("save file frag for recv unit list ", __LINE__);
        m_file_frag_address[m_File->get_File_Name()] = file_save_unit;
        
        //유닛들에게 전송하는것을 마침
        return 0;
    }
    catch(Cuma_Error& e)
    {
        e.show_error_string();
        return -1;
    }
}

int Cuma_Main::mf_command_req_file_test()
{
    
    try{

        //파일 이름이 있는지 확인
        Cuma_Debug("check file name is exsist...", __LINE__);
        if(m_File->get_File_Name().isEmpty())
        {
            throw Cuma_Error("File name is missing",__LINE__);
        }

        Cuma_Debug("check file frag address is empty ..", __LINE__);
        if(m_file_frag_address[m_File->get_File_Name()].isEmpty())
        {
            throw Cuma_Error("m_file_frag_address can't find file_name", __LINE__);
        }

        //파일 frag를 리퀘스트 프로토콜을 전송함
        Cuma_Debug("send file frag to unit ..", __LINE__);
        for(uint i = 0; i < m_file_frag_address[m_File->get_File_Name()].count() ; i++)
        {
            Cuma_Debug("req file frag to unit : "+  QString::number(i), __LINE__);

            QMap<uint32_t, QVector<uint32_t>>& frag_by_unit_addr = m_file_frag_address[m_File->get_File_Name()];

            uint32_t unit_pid = frag_by_unit_addr[i].first();

            QSharedPointer<Cuma_Main> unit = f_find_unit_from_inside_timeout_unit(unit_pid);

            if(unit == nullptr)
            {
                throw Cuma_Error("can't find unit SharedPointer", __LINE__);
            }

            client_send(cuma_protocol::req_file_binary_read_protocol("test.txt", i, m_Pid), unit);
        }
        return 0;
    }
    
    catch(Cuma_Error& e)
    {
        e.show_error_string();
        return -1;
    }
}

int Cuma_Main::mf_command_trace_pass_test()
{
    try{

        //모든 유닛들에게 bypass하는 테스트
        QJsonObject bypass_obj = cuma_protocol::req_bypass_protocol(m_Pid, m_limit_bypass_count);

        if(bypass_obj["bypass_count"].toInt() < 1)
        {
            throw Cuma_Error("bypass_count is under 0", __LINE__);
        }

        bypass_obj["bypass_count"] = bypass_obj["bypass_count"].toInt() - 1;

        //모든 유닛들에게 spread함
        for(QSharedPointer<Cuma_Main> it: m_Cuma_unit_inside_timeout_unit_list)
        {
            client_send(bypass_obj, it);
        }

        return 0;
    }

    catch(Cuma_Error& e)
    {
        e.show_error_string();
        return -1;
    }
}

int Cuma_Main::mf_command_req_file_exsist()
{
    QJsonObject protocol = cuma_protocol::req_is_file_exsist_protocol(m_File->get_File_Name(), m_File->get_File_Index(), m_Pid);

    //모든 유닛들에게 전송함
    for(QSharedPointer<Cuma_Main> it: m_Cuma_unit_inside_timeout_unit_list)
    {
        client_send(protocol, it);
    }

    return 0;
}

void Cuma_Main::sl_start_idle()
{
    m_unit_idle = true;
    Cuma_Debug("Unit  : " + QString::number(m_Pid) + "is idle");
}

void Cuma_Main:: mf_command_set_unit_bypass_count(uint32_t count)
{
    //유닛의 바이패스 카운트를 설정함
    m_limit_bypass_count = count;
}
