#include "../cuma_main.h"

int Cuma_Main::f_upload_file_frag_from_unit(QJsonObject o)
{
    try
    {
        //해당 파일을 요청한 uid의 유닛 포인터를 반환함
        QSharedPointer<Cuma_Main> req_unit = m_Cuma_unit_inside_timeout_unit_list[static_cast<uint32_t>(o["pid"].toInt())];

        //만약 파일을 요청한 uid의 유닛포인터가 m_Cuma_unit_list의 검색에 없을경우 예외처리
        if (req_unit == nullptr)
        {
            throw Cuma_Error("No Such Unit in m_Cuma_unit_list while replying the unit Pointer", __LINE__, m_Pid);
        }

        //요청된 저장된 파일을 읽음
        QByteArray frag_binary = m_File->get_fragment_slite(o["file_index"].toInt());

        //해당 유닛에 전송함
        req_unit->s_recv(cuma_protocol::reply_file_binary_read_protocol(o["file_name"].toString(), o["file_index"].toInt(), m_Pid, frag_binary));

    }
    catch(Cuma_Error& e)
    {
        e.show_error_string();
        return -1;
    }
}

int Cuma_Main::f_reply_upload_file_frag_to_unit(const QJsonObject o)
{
    try
    {
        //유닛의 목적지가 어디인지
        uint32_t unit_to  =  static_cast<uint32_t>(o["To"].toInt());

        uint32_t next_unit_pid = nullptr;

        //만약 자기 유닛에게 오지 않았을 경우 적혀진 경로대로 탐색을 해서 자기 유닛이후의 다음 유닛에게 전송을 함
        if (unit_to != m_Pid)
        {
            //적혀진 경로 를 json_array로 변환함
            QJsonArray bypass_array= o["bypass"].toArray();

            //bypassarray에 있는 유닛들을 찾음
            for (int i = 0; i< bypass_array.count(); i++)
            {
                //만약 해당 array가 자기 pid랑 일치할경우 next_unit_pid에 값을 할당함
                if (bypass_array.at(i).toInt() == m_Pid)
                {
                    next_unit_pid = static_cast<uint32_t>(bypass_array[i+1].toInt());
                }
            }

            //next_unit_pid가 없을경우 예외처리
            if (next_unit_pid == nullptr)
            {
                throw Cuma_Error("f_reply_upload_file_frag_to_unit::no such unit from destination", __LINE__, m_Pid);
            }

            //next_unit_pid의 유닛들의 포인터를 통해 전송함
            QSharedPointer<Cuma_Main> next_bypass_unit = nullptr;

            //유닛의 pid를 찾아서 next_bypass_unit에 넣음
            for (QVector<QSharedPointer<Cuma_Main>>::iterator it = m_Cuma_unit_inside_timeout_unit_list.first(); it != m_Cuma_unit_inside_timeout_unit_list.end(); it++)
            {
                if ((*it)->mf_get_pid == next_unit_pid)
                {
                    next_bypass_unit = (*it);
                    break;
                }
            }

            //받은 recv프로토콜을 그대로 다음 유닛에게 전송
            next_bypass_unit->s_recv(o);
        }

        //만약 자기 유닛일 경우 전송을 받아서 해석을 한후 저장함
        else
        {
            //어디유닛에서 전송을 했는지 적음
            uint32_t    unit_from = static_cast<uint32_t>(o["From"].toInt());

            //파일이름을 표시함
            QString     file_name = o["file_name"].toString();
            uint32_t    file_index = (uint32_t)o["file_index"].toInt();
            uint32_t    file_size = (uint32_t)o["file_byte"].toInt();
            QString     frag_string = o["file_frag"].toString();
            QByteArray  file_frag(frag_string.constData(), frag_index);

            //디버그 표시
            Cuma_Debug("f_reply_upload_file_frag_to_unit", __LINE__);
            Cuma_Debug("File_Frag_size : " + QString::number(file_size), __LINE__);
            Cuma_Debug("File_Frag_info : ", + "size : " + QString::number(file_frag.count()), __LINE__);

            //만약 frag인덱스와 file_frag의 사이즈가 안맞을 경우 예외 처리 보냄
            //이미 파일체크 프로세스가 바이패스 프로세스를 호출해서 이미 ping이 남아있는데 없다는것은 예외처리를 통해서 exception문을 호출해야함
            if (frag_index != file_frag.count())
            {
                throw Cuma_Error("f_reply_upload_file_frag_to_unit error frag_index != file_frag.count() : recv frag_index and file_frag_count is not match", __LINE__, m_Pid);
            }

            //리턴된 파일의 바이너리를 읽고 저장함
            int ret_type = m_File->save_File_Frag(file_frag, file_name, file_index);

            //만약 파일이 이미 존재할경우 continue함
            if ( ret_type == Cuma_File::Cuma_File_Status::C_F_Exsist)
            {
                continue;
            }

            //리턴함
            return 0;
        }
    }
    catch(Cuma_Error& e)
    {
        e.show_error_string();
        return -1;
    }
}

int Cuma_Main::f_download_file_frag_from_unit(QJsonObject o)
{
    try
    {
        //가지고 온 파일을 저장함
        if (m_File->save_File_Frag(o["file_frag"].toString().toUtf8(), o["file_name"].toString(), static_cast<uint32_t>(o["file_index"].toInt())) < 0)
        {
            Cuma_Debug("Can't open file", __LINE__);
            return 0;
        }

        //전송을 한 유닛의 sharedpointer를 가짐
        QSharedPointer<Cuma_Main> prepare_send_unit = nullptr;

        //유닛의 pid를 찾아서 prepare_send_unit 넣음
        for (QVector<QSharedPointer<Cuma_Main>>::iterator it = m_Cuma_unit_inside_timeout_unit_list.first(); it != m_Cuma_unit_inside_timeout_unit_list.end(); it++)
        {
            if ((*it)->mf_get_pid == next_unit_pid)
            {
                next_bypass_unit = (*it);
                break;
            }
        }

        //만약 prepare_send_unit이 nullptr일경우 예외처리 exception문을 호출
        if (prepare_send_unit == nullptr)
        {
            throw Cuma_Error("f_reply_upload_file_frag_to_unit error frag_index != file_frag.count() : recv frag_index and file_frag_count is not match", __LINE__, m_Pid);
        }

        //전송을 한 유닛에게 reply_download_file_frag 프로토콜을 전송함
        emit prepare_send_unit->s_recv(cuma_protocol::reply_file_binary_save_protocol(o["file_name"].toString(), m_Pid, o["file_index"].toInt()));

    }

    catch(Cuma_Error& e)
    {
        e.show_error_string();
        return -1;
    }
}

int Cuma_Main::f_reply_download_file_frag_to_unit(QJsonObject o)
{
    //reply된 download_frag_file를 체크하고 파일이 저장을 한 유닛의 pid를 File_Frag의 메타데이터에 저장함
    if ( !o["reply"].isNull() && o["reply"].toBool() == true)
    {
        Cuma_Debug("Unit : " + static_cast<uint32_t>(o["From"].toInt()) + "save file_frag  " + o["file_name"].toString() + ": " + o["file_index"].toInt());
    }

    //만약 아닐경우 error를 출력함(이건 reply중요도가 떨어지기때문에 굳이 exception을 던지지 않음)
    else
    {
        Cuma_Debug("f_reply_download_file_Frag is error", __LINE__, m_Pid);
    }
    return 0;
}

int Cuma_Main::f_check_file_frag_to_unit(QJsonObject o)
{
    //만약 파일이 없을경우 다른 유닛들에게 파일을 전송하고 reply로 파일이 없고 현재 bypass count대로 탐색중이라는것을 알림

    QString req_frag_name = o["file_name"].toString();
    uint32_t req_frag_index = static_cast<uint32_t>(o["file_index"].toInt());

    int32_t f_flag = m_File->read_file_frag(req_frag_name, req_frag_index);

    //파일이 없을경우 다른 유닛에게 bypass 프로토콜을 실행함
    if ( f_flag == Cuma_File::Cuma_File_Status::C_F_Dir_Not_Open || f_flag == Cuma_File::Cuma_File_Status::C_F_not_open)
    {
        //바이패스 프로토콜을 실행함
        if (f_over_bypass(o) < 0)
        {
            //만약 바이패스 프로토콜이 -1일경우 바이패스 리미트가 초과이므로 파일이 없다는 프로토콜을 만들어서 위에 있는 유닛으로 bypass 함
            QJsonObject reply_bypass_protocol = cuma_protocol::reply_is_file_exsist_protocol(o["file_index"].toInt(), m_PId, false);
            reply_bypass_protocol["bypass"] = o["bypass"].toArray();
            reply_bypass_protocol["bypass_limit_count"] = o["bypass_limit_count"].toInt();
            reply_bypass_protocol["bypass_count"] = o["bypass_count"].toInt();

            //bypass 체인으로 올라가는데 만약 에러가 발생시 예외 처리 함
            if (f_reply_over_bypass_limit(reply_bypass_protocol) < 0)
            {
                throw Cuma_Error("bypass_chain_error", __LINE__, m_Pid);
            }
        }
    }

    //파일이 있을경우 파일이 있다는 프로토콜을 보냄
    else
    {
        //전송을 한 유닛의 sharedpointer를 가짐
        QSharedPointer<Cuma_Main> prepare_send_unit = nullptr;

        //유닛의 pid를 찾아서 prepare_send_unit 넣음
        for (QVector<QSharedPointer<Cuma_Main>>::iterator it = m_Cuma_unit_inside_timeout_unit_list.first(); it != m_Cuma_unit_inside_timeout_unit_list.end(); it++)
        {
            if ((*it)->mf_get_pid == next_unit_pid)
            {
                prepare_send_unit = (*it);
                break;
            }
        }

        //프로토콜을 전송함
        emit prepare_send_unit->s_recv(cuma_protocol::reply_is_file_exsist_protocol(req_frag_name, m_Pid, true));

    }
    return 0;
}

int Cuma_Main::f_reply_check_file_frag_to_unit(const QJsonObject o)
{
    try{
        //bypass의 프로토콜은 bypass_count 끝날때까지 리턴값을 못받는것으로 산정함

        //만약 bypass 프로토콜인지 확인함
        if (!o["bypass"].isNull())
        {
            uint32_t temp_bypass_count = o["bypass_count"].toInt();

            //만약 자기 로 오는 bypass 프로토콜일 경우 파일이 존재하는지 확인
            if(o["To"].toInt() == m_Pid)
            {
                //만약 파일 frag가 존재할경우 path 블록을 만들어서 저장함
                if (o["exsist"].toBool() == true)
                {
                    //bypass의 유닛들을 저장할 주소
                    QVector<uint32_t> path_unit_block;

                    QJsonArray bypass_unit_array = o["bypass"].toArray();

                    //bypass_unit_array를 path_unit_block으로 넣음
                    foreach(QJsonValue& e, bypass_unit_array)
                    {
                        path_unit_block.append(static_cast<uint32_t>(e.toInt()));
                    }

                    //path_unit_block을 frag_address 저장함
                    QMap<uint32_t, QVector<uint32_t>> frag_index_address;
                    frag_index_address[static_cast<uint32_t>(o["file_index"].toInt())] = path_unit_block;

                    //m_file_frag_address 저장함
                    m_file_frag_address[o["file_name"].toString()] = frag_index_address;
                }

                //만약 파일이 못찾을 경우 frag_address 저장
                else
                {
                    //못찾은 경우로 해당 frag_index에 nullptr을 저장함
                    QMap<uint32_t, QVector<uint32_t>> frag_index_address;
                    frag_index_address[static_cast<uint32_t>(o["file_index"].toInt())] = nullptr;

                    //m_file_frag_address 저장함
                    m_file_frag_address[o["file_name"].toString()] = frag_index_address;
                }
            }

            //자기로 오는 bypass 프로토콜이 아닐 경우 f_repoly_over_bypass_limit()를 호출
            else
            {
                if (f_reply_over_bypass_limit(o) < 0)
                {
                    throw Cuma_Error("f_reply_over_bypass_limit error", __LINE__, m_Pid);
                }
            }
        }

        //만약 바이패스가 아니면
        else
        {
            //만약 파일이 있으면 해당파일을 m_file_frag_address에 저장함
            if (o["exsist"].toBool() == true)
            {
                QMap<uint32_t, QVector<uint32_t>> frag_index_address;

                frag_index_address[static_cast<uint32_t>(o["file_index"].toInt())] = static_cast<uint32_t>(o["From"].toInt());

                //m_file_frag_address 저장함
                m_file_frag_address[o["file_name"].toString()] = frag_index_address;
            }

            //만약 파일이 없으면 m_file_frag_address 에 0을 저장함
            else
            {
                //못찾은 경우로 해당 frag_index에 0을 저장함
                QMap<uint32_t, QVector<uint32_t>> frag_index_address;
                frag_index_address[static_cast<uint32_t>(o["file_index"].toInt())] = 0;

                //m_file_frag_address 저장함
                m_file_frag_address[o["file_name"].toString()] = frag_index_address;
            }
        }
        return 0;
    }

    catch(Cuma_Error& e)
    {
        e.show_error_string();
        return -1;
    }
}

int Cuma_Main::f_over_bypass(QJsonObject o)
{
    //자기가 가지고 있는 다른 유닛들에게 프로토콜을 전송함

    //만약 기존의 바이패스가 없는 경우 새로운 bypass를 삽입함
    if(o["bypass"].isNull())
    {
        //새로운 바이패스 프로토콜을 생성함
        o["bypass_count"] = o["bypass_limit_count"].toInt();

        //바이패스 에 자기 PId를 넣음
        o["bypass"] = o["bypass"].toArray().append(m_Pid);
    }

    //만약 bypass의 카운트가 0일경우 이미 리미트까지 갔다는 의미
    if(o["bypass_count"].toInt() == 0)
    {
        //bypass 카운트가 0이므로 -1를 리턴함
        return -1;
    }

    //bypass_count를 -1함
    o["bypass_count"] -= 1;

    //ping으로 스크리밍이 된 모든 유닛들에게 bypass함
    for (QVector<QSharedPointer<Cuma_Main>>::iterator it = m_Cuma_unit_inside_timeout_unit_list.first();
         it != m_Cuma_unit_inside_timeout_unit_list.end();
         it++)
    {
        emit (*it)->s_recv(o);
    }

    return 0;
}

int Cuma_Main::f_reply_over_bypass_limit(QJsonObject o)
{

    //리플라이가 된 프로토콜을 다른 유닛에게 넘기는 프로토콜

    try{

        //bypass count가 초과가 됬는지 확인
        if(o["bypass_count"].toInt() < 0)
        {
            //만약 초과가 됬다면 에러가 발생한것으로 extion을 발생함
            throw Cuma_Error("f_reply_over_bypass_limit: bypass_ount < bypass_limit" , __LINE__, m_Pid);
        }

        //초과가 되지 않았다면 bypass 안에 자기 pid의 체인 위에 있는 유닛에게 전송함
        QJsonArray bypass_chain =  o["bypass"].toArray();

        //전송하려는 유닛의 pid를 찾음
        uint32_t send_unit_id;

        //전송이 정상적으로 완료가 되었는지
        bool bypass_success = false;

        //전송하려는 유닛 주소
        QSharedPointer<Cuma_Main> send_unit = nullptr;

        //bypass_chain을 통해서 전송할 다음 바이패스 체인을 찾음
        for (uint32_t i = 0; i < bypass_chain.count() ; i++)
        {
            //만약 바이패스 체인에 자신을 찾았을 경우 다음 바이패스 체인인의 pid를 찾음
            if(bypass_chain.at(i) == m_Pid)
            {
                if(i == 0)
                {
                    //만약 i 가 0일때 자기자신이므로  0을 리턴함
                    return 0;
                }

                //뒤의 바이패스 체인을 찾음
                send_unit_id = bypass_chain.at(i - 1).toInt();
                break;
            }
        }

        //뒤의 바이패스 체인을 자기 유닛 리스트에서 찾아서 전송함
        for(QVector<QSharedPointer<Cuma_Main>>::iterator it = m_Cuma_unit_inside_timeout_unit_list.begin(); it != m_Cuma_unit_inside_timeout_unit_list.end(); it++)
        {
            //만약 유닛 리스트중에 보내려는 유닛을 찾았을떄
            if ((*it)->mf_get_pid() == send_unit_id)
            {
                send_unit = (*it);
                bypass_success = true;
                break;
            }
        }

        //만약 바이패스 유닛들을 못찾았을때 exception을 던짐
        if(bypass_success == false || unit_addr = nullptr)
        {
            throw Cuma_Error("f_reply_over_bypass_limit : Can't find bypass unit in this unit list", __LINE__, m_Pid);
        }

        //프로토콜을 전송함
        emit send_unit->s_recv(o);

        return 0;
    }

    catch(Cuma_Error& e)
    {
        e.show_error_string();
        return -1;
    }
}
