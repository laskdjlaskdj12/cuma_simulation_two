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

        //모든 유닛들을 ping으로 스크리밍해서 리스트 안에 넣음
        if(m_Cuma_unit_list.count() == 0)
        {
            throw Cuma_Error("mf_command_ping_test: Cuma_unit_list is empty", __LINE__, m_Pid);
        }

        //모든 유닛들에게 전송함
        foreach(QSharedPointer<Cuma_Main> p, m_Cuma_unit_unit_list)
        {
            emit p->s_recv(cuma_protocol::req_ping_protocol(m_Pid));
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
        if( ! (m_Cuma_unit_inside_timeout_unit_list.count() > 0))
        {
            throw Cuma_Error("mf_command_spread_test: no unit found in Cuma_unit_inside_timeout_list ", __LINE__, m_Pid);
        }

        //전송할 파일 바이너리를 읽음
        QByteArray spread_binary =  m_File->get_File_binary();

        //만약 spread_binary가 nullptr일경우 예외처리
        if(spread_binary == nullptr)
        {
            throw Cuma_Error("mf_command_spread_test: Can't of read File bianry", __LINE__, m_Pid);
        }

        //전송할 파일들을 유닛수대로 파편화를 함
        m_File->set_File_Frag_Count(m_Cuma_unit_inside_timeout_unit_list.count());

        uint i = 0;

        //파일 정보에 대한 블록을 만듬
        QJsonObject file_info_block;
        file_info_block["file_frag_count"] = m_Cuma_unit_inside_timeout_unit_list.count();
        file_info_block["file_binary_size"] = m_File->get_File_binary().count();

        //파일의 정보 블록을 저장함
        m_file_info_block[m_File->get_File_Name()] = file_info_block;

        //파일 유닛들의 send info 저장함
        QMap<uint32_t, QVector<uint32_t>> file_save_unit;

        //파일 파편화를 모든 유닛들에게 전송함
        foreach(QSharedPointer<Cuma_Main>& p, m_Cuma_unit_unit_list)
        {
            //파편화에 대한 json을 만듬
            QJsonObject send_frag_binary;
            send_frag_binary["file_frag"] = m_File->get_File_Frag_By_Index(i);
            send_frag_binary["file_name"] = m_File->get_File_Name();
            send_frag_binary["file_index"] = i;
            send_frag_binary["file_info_block"] = file_info_block;

            emit p->s_recv(cuma_protocol::req_file_binary_save_protocol(send_frag_binary, m_Pid));

            QVector<uint32_t> file_Frag_index;
            file_Frag_index.append(p->mf_get_pid());

            file_save_unit[i] = file_Frag_index;

            i++;
        }

        //File_frag를 수신한 유닛들의 주소들을 frag인덱스별로 저장함
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

        //file_frag_address의 블록이 이미 있을때 해당 유닛에게 블록을 읽는 커맨드
        //반드시 파일별 m_file_frag_address가 미리 있어야함

        //읽을 바이패스 파일의 블록을 저장할 리스트
        QVector<uint32_t> req_file_frag_index;

        QJsonObject read_list = m_file_info_block[m_File->get_File_Name()];

        uint32_t read_list_count = dynamic_cast<uint32_t> (read_list["file_index"].toInt());

        //해당 파일 블록이 있는지 체크함
        QMap<uint32_t, QVector<uint32_t>>& file_block_list = m_file_frag_address[m_File->get_File_Name()];

        //frag_index별로 유닛 벡터에 유닛들이 있는지 확인함
        for(uint32_t i = 0; i < read_list_count ; i++)
        {
            QVector<uint32_t> unit_list = file_block_list[i];

            if ( unit_list.isEmpty() == true)
            {
                //읽을 파일블록 인덱스를 read_list에 넣음
                req_file_frag_index.append(i);
            }
        }

        //만약 읽을 파일 address가 없을경우 Cuma_Error를 리턴함
        if(req_file_frag_index.isEmpty() == false)
        {
            throw Cuma_Error("mf_command_req_file_test : Need to read All File_Block", __LINE__, m_Pid);
        }

        //파일 address에 적혀있는 유닛들에게 전송함
        for(uint32_t i = 0; i < read_list_count ; i++)
        {
            QVector<uint32_t>& unit_list = file_block_list[i];

            QSharedPointer<Cuma_Main>& ready_send_unit = f_find_unit_from_inside_timeout_unit(unit_list);

            //요청 프로토콜을 unit에게 전송함
            emit ready_send_unit->s_recv(cuma_protocol::req_file_binary_read_protocol(m_File->get_File_Name(),
                                                                                      req_file_frag_index[i],
                                                                                      m_Pid));
        }

        uint32_t timeout_t = 0;

        do
        {
            //타임아웃이 될때까지 wait
            QThread::wait(1000);
            timeout_t ++;

        }while(timeout_t < 10);

        //만약 모든 frag_index의 리스트들이 돌아오지 않았을 경우 Cuma_Error를 리턴함
        if(file_block_list.count() != read_list_count)
        {
            Cuma_Error("all frag_index_list is not complete", __LINE__, m_Pid);

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
    //모든 유닛들에게 파일이 있는지 체크를 하는 실험

    //frag_address을 전부 flush함
    m_file_frag_address[m_File->get_File_Name()].clear();

    //모든 유닛에게 파일 메타데이터 인덱스를 요청함
    foreach(QSharedPointer<Cuma_Main>& p, m_Cuma_unit_inside_timeout_unit_list)
    {
        emit p->s_recv(cuma_protocol::req_is_file_exsist_protocol(m_File->get_File_Name(), m_Pid));
    }

    //파일 메타데이터 인덱스가 도착했는지 체크함
    if(m_file_info_block[m_File->get_File_Name()].isEmpty())
    {
        throw Cuma_Error("Can't find any File_index");
    }

    //인덱스가 도착했으면 인덱스를 읽어서 is_file_frag_exsist 프로토콜로 파일frag를 체크함

    for(uint32_t frag_index = 0;
        frag_index < m_file_info_block[m_File->get_File_Name()]["file_frag_count"].toInt();
        frag_index++)
    {
        //is_file_exsist_protocol을 모든 유닛에게 전송
        emit p->s_recv(cuma_protocol::req_is_file_exsist_protocol(frag_index, m_Pid));
    }
}

void Cuma_Main::mf_command_set_unit_bypass_count(uint32_t count)
{
    //유닛의 바이패스 카운트를 설정함
    m_limit_bypass_count = count;
}
