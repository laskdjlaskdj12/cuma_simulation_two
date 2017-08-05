#include "../cuma_main.h"

int Cuma_Main:: f_upload_file_frag_from_unit(QJsonObject o)
{
    try
    {
        //해당 파일을 요청한 uid의 유닛 포인터를 반환함
        Cuma_Debug("return request unit pid and unit pointer", __LINE__);
        QSharedPointer<Cuma_Main> req_unit = m_Cuma_unit_inside_timeout_unit_list[static_cast<uint32_t>(o["From"].toInt())];

        //만약 파일을 요청한 uid의 유닛포인터가 m_Cuma_unit_list의 검색에 없을경우 예외처리
        Cuma_Debug("check req uid is not in m_Cuma_unit_list", __LINE__);
        if (req_unit == nullptr)
        {
            throw Cuma_Error("No Such Unit in m_Cuma_unit_list while replying the unit Pointer", __LINE__, m_Pid);
        }

        QString file_name = o["file_name"].toString();
        uint32_t file_index = static_cast<uint32_t>(o["file_index"].toInt());

        //요청된 저장된 파일을 읽음
        Cuma_Debug("read request file", __LINE__);

        //보낼 응답 프로토콜을 미리 작성
        QJsonObject reply_protocol;
        switch(m_File->read_file_frag(file_name, file_index))
        {

        //만약 no_err일 경우
        case Cuma_File::C_F_no_err:
        {
            Cuma_Debug("C_F_no_err", __LINE__);
            //reply_protocol에 frag 바이너리를 넣음
            QByteArray frag_binary = m_File->get_File_binary();
            reply_protocol = cuma_protocol::reply_file_binary_read_protocol(file_index, file_name, m_Pid, frag_binary);
        }
            break;

            //파일이 존재하지않을 경우
        case Cuma_File::C_F_NOT_EXSIST:
        {
            Cuma_Debug("C_F_NOT_EXSIST", __LINE__);
            //파일이 존재하지 않는다는 프로토로콜을 넣음
            reply_protocol = cuma_protocol::reply_file_binary_read_protocol(file_index, file_name, m_Pid, "NONE");
            reply_protocol["Error_Type"] = "File";
            reply_protocol["Error"] = Cuma_File::C_F_NOT_EXSIST;

        }
            break;
        case Cuma_File::C_F_not_open:
        {
            Cuma_Debug("C_F_not_open", __LINE__);
            reply_protocol = cuma_protocol::reply_file_binary_read_protocol(file_index, file_name, m_Pid, "NONE");
            reply_protocol["Error_Type"] = "File";
            reply_protocol["Error"] = Cuma_File::C_F_not_open;
        }
            break;

        default:
        {
            Cuma_Debug("default", __LINE__);
            reply_protocol = cuma_protocol::reply_file_binary_read_protocol(file_index, file_name, m_Pid, "NONE");
            reply_protocol["Error_Type"] = "File";
            reply_protocol["Error"] = Cuma_File::C_F_error;
        }

        }

        //해당 유닛에 전송함
        Cuma_Debug("send reply", __LINE__);

        if(o.find("bypass") != o.end())
        {
            req_unit = find_bypass_reply_obj(o["bypass"].toArray());
            server_bypass_send(reply_protocol, o, req_unit);
        }
        else
        {
           server_send(reply_protocol, req_unit);
        }

        return 0;
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
        Cuma_Debug("check unit destination", __LINE__);

        //파일이름을 표시함
        Cuma_Debug("save file_info", __LINE__);
        uint32_t    unit_from = static_cast<uint32_t>(o["From"].toInt());
        QString     file_name = o["file_name"].toString();
        uint32_t    file_index = (uint32_t)o["file_index"].toInt();
        uint32_t    file_size = (uint32_t)o["file_byte"].toInt();
        QString     frag_string = o["file_frag"].toString();
        QByteArray  file_frag(frag_string.toUtf8());

        //디버그 표시
        Cuma_Debug("show debug data", __LINE__);
        Cuma_Debug("f_reply_upload_file_frag_to_unit", __LINE__);
        Cuma_Debug("File_Frag_size : " + QString::number(file_size), __LINE__);
        Cuma_Debug("File_Frag_info : size : " + QString::number(file_frag.count()), __LINE__);

        Cuma_Debug("check frag_index is not match file_frag count", __LINE__);

        //리턴된 파일의 바이너리를 읽고 저장함
        Cuma_Debug("read return file binary and save", __LINE__);
        int ret_type = m_File->save_File_Frag(file_frag, file_name, file_index);

        //만약 파일이 이미 존재할경우 continue함
        Cuma_Debug("check if file is exsist", __LINE__);
        if ( ret_type == Cuma_File::Cuma_File_Status::C_F_Exsist)
        {
            Cuma_Debug("file is already exsist", __LINE__);
        }

        //리턴함
        return 0;
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
        Cuma_Debug("save recv protocol file ", __LINE__);
        if (m_File->save_File_Frag(o["file_frag"].toString().toUtf8(), o["file_name"].toString(), o["file_index"].toInt()) < 0)
        {
            Cuma_Debug("Can't open file", __LINE__);
            return 0;
        }

        //전송을 한 유닛의 sharedpointer를 가짐
        QSharedPointer<Cuma_Main> prepare_send_unit = nullptr;

        //유닛의 pid를 찾아서 prepare_send_unit 넣음
        Cuma_Debug("find unit shared pointer in m_Pid", __LINE__);
        prepare_send_unit = f_find_unit_from_inside_timeout_unit (o["From"].toInt());

        //만약 prepare_send_unit이 nullptr일경우 예외처리 exception문을 호출
        Cuma_Debug("check find send target unit", __LINE__);
        if (prepare_send_unit == nullptr)
        {
            throw Cuma_Error("f_reply_upload_file_frag_to_unit error frag_index != file_frag.count() : recv frag_index and file_frag_count is not match ", __LINE__, m_Pid);
        }

        //전송을 한 유닛에게 reply_download_file_frag 프로토콜을 전송함
        Cuma_Debug("send reply_download_file_frag protocol", __LINE__);
        QJsonObject reply_protocol = cuma_protocol::reply_file_binary_save_protocol(m_Pid, o["file_frag"].toString(), o["file_index"].toInt());

        if(o.find("bypass") != o.end())
        {
            prepare_send_unit = find_bypass_reply_obj(o["bypass"].toArray());
            server_bypass_send(reply_protocol, o, prepare_send_unit);
        }
        else
        {
           server_send(reply_protocol, prepare_send_unit);
        }

        return 0;
    }

    catch(Cuma_Error& e)
    {
        e.show_error_string();
        Cuma_Error("unit id : " + QString::number(o["From"].toInt()) + " pid : " + QString::number(m_Pid), __LINE__).show_error_string();
        return -1;
    }
}

int Cuma_Main::f_reply_download_file_frag_to_unit(QJsonObject o)
{
    //reply된 download_frag_file를 체크하고 파일이 저장을 한 유닛의 pid를 File_Frag의 메타데이터에 저장함
    Cuma_Debug("check download_frag_file protocol ", __LINE__);
    if ( !(o["reply"].isNull()) && o["reply"].toBool() == true)
    {
        Cuma_Debug("save unit pid to file_frag metadata", __LINE__);
        Cuma_Debug("Unit : " + QString::number(static_cast<uint32_t>(o["From"].toInt())) + " save file_frag  " + o["file_name"].toString() + ": " + QString::number(o["file_index"].toInt()));

    }

    //만약 아닐경우 error를 출력함(이건 reply중요도가 떨어지기때문에 굳이 exception을 던지지 않음)
    else
    {
        Cuma_Debug("f_reply_download_file_Frag is error", __LINE__);
    }
    return 0;
}

int Cuma_Main::f_check_file_frag_to_unit(QJsonObject o)
{
    //만약 파일이 없을경우 다른 유닛들에게 파일을 전송하고 reply로 파일이 없고 현재 bypass count대로 탐색중이라는것을 알림

    QString req_frag_name = o["file_name"].toString();
    uint32_t req_frag_index = static_cast<uint32_t>(o["file_index"].toInt());

    QJsonObject protocol;
    Cuma_Debug("read file frag ["+ req_frag_name +"] : " + QString::number(req_frag_index), __LINE__);
    int32_t f_flag = m_File->read_file_frag(req_frag_name, req_frag_index);

    //파일이 없을경우 파일이 없다는 reply를 전송
    Cuma_Debug("check file is exsist", __LINE__);
    if ( f_flag == Cuma_File::Cuma_File_Status::C_F_Dir_Not_Open || f_flag == Cuma_File::Cuma_File_Status::C_F_not_open)
    {
        Cuma_Debug("Send None File exsist..", __LINE__);
        protocol = cuma_protocol::reply_is_file_exsist_protocol(req_frag_name, m_Pid, req_frag_index, false);
    }

    //파일이 있다는 프로토콜을 전송
    else
    {
        Cuma_Debug("Send File exsist..", __LINE__);
        protocol = cuma_protocol::reply_is_file_exsist_protocol(req_frag_name, m_Pid, req_frag_index, true);
    }

    //해당 유닛을 찾아서 전송
    QSharedPointer<Cuma_Main> send_unit = f_find_unit_from_inside_timeout_unit(o["From"].toInt());

    if(o.find("bypass") != o.end())
    {
        send_unit = find_bypass_reply_obj(o["bypass"].toArray());
        server_bypass_send(protocol, o, send_unit);
    }
    else
    {
       server_send(protocol, send_unit);
    }

    return 0;
}

int Cuma_Main::f_reply_check_file_frag_to_unit(const QJsonObject o)
{

    //만약 파일이 있으면 해당파일을 m_file_frag_address에 저장함
    if (o["exsist"].toBool() == true)
    {
        Cuma_Debug("the bypass protocol is mine and file is exsist", __LINE__);

        QVector<uint32_t> bypass_unit_list;
        bypass_unit_list.append(o["From"].toInt());

        QMap<uint32_t, QVector<uint32_t>> frag_index_address;
        frag_index_address[static_cast<uint32_t>(o["file_index"].toInt())] = bypass_unit_list;

        //m_file_frag_address 저장함
        m_file_frag_address[o["file_name"].toString()] = frag_index_address;
    }

    //만약 파일이 없으면 m_file_frag_address 에 0을 저장함
    else
    {
        Cuma_Debug("the bypass protocolis mine and file is not exsist ", __LINE__);
        QVector<uint32_t> bypass_unit_list;

        //못찾은 경우로 해당 frag_index에 0을 저장함
        QMap<uint32_t, QVector<uint32_t>> frag_index_address;
        frag_index_address[static_cast<uint32_t>(o["file_index"].toInt())] = bypass_unit_list;

        //m_file_frag_address 저장함
        m_file_frag_address[o["file_name"].toString()] = frag_index_address;
    }
    return 0;

}

int Cuma_Main::f_over_bypass(QJsonObject o)
{

    //bypass 핸들러 실행전에 정보 등록
    Cuma_Debug("check bypass is exsist ", __LINE__);
    if(o["bypass"].isNull())
    {
        Cuma_Error("Bypass unit array is not exsist ", __LINE__).show_error_string();
        return 0;
    }

    //만약 bypass의 카운트가 0일경우 이미 리미트까지 갔다는 의미
    Cuma_Debug("check bypass count is exsist ", __LINE__);
    if(o["bypass_count"].toInt() < 1)
    {
        Cuma_Debug("bypass count is 0 ", __LINE__);

        return 1;
    }

    //bypass_count를 -1함
    Cuma_Debug("remove 1 in bypass count ", __LINE__);
    o["bypass_count"] = o["bypass_count"].toInt() - 1;

    //바이패스 의 방향
    o["bypass_reply"] = false;

    //ping으로 스크리밍이 된 모든 유닛들에게 bypass함
    uint send_count = 0;
    for (QSharedPointer<Cuma_Main> it : m_Cuma_unit_inside_timeout_unit_list)
    {
        Cuma_Debug("spread bypass protocol to unit : " + QString::number(it->mf_get_pid()), __LINE__);
        //바이패스가 온 유닛은 제외
        if ( it->mf_get_pid() == o["From"].toInt())
        {
            continue;
        }

        //From을 자기자신의 pid로 변경함
        QJsonObject send_protocol = o;
        send_protocol["From"] = static_cast<int>(m_Pid);
        emit it->s_recv(send_protocol);

        send_count ++;
    }

    if(send_count == 0)
    {
        Cuma_Debug("There is no Unit to send", __LINE__);
        return 2;
    }

    return 0;
}

int Cuma_Main::f_reply_over_bypass_limit(QJsonObject o)
{

    //리플라이가 된 프로토콜을 다른 유닛에게 넘기는 프로토콜
    Cuma_Debug("reply over bypass limit ", __LINE__);

    try{

        //bypass count가 초과가 됬는지 확인
        Cuma_Debug("check bypass count is over ", __LINE__);
        if(o["bypass_count"].toInt() < 0)
        {
            Cuma_Debug("bypass_count is < 0 ", __LINE__);
            //만약 초과가 됬다면 에러가 발생한것으로 extion을 발생함
            throw Cuma_Error("f_reply_over_bypass_limit: bypass_ount < bypass_limit" , __LINE__, m_Pid);
        }

        //초과가 되지 않았다면 bypass 안에 자기 pid의 체인 위에 있는 유닛에게 전송함
        Cuma_Debug("add my pid in bypass chain", __LINE__);
        QJsonArray bypass_chain =  o["bypass"].toArray();

        //전송하려는 유닛의 pid를 찾음
        uint32_t send_unit_id;

        //전송하려는 유닛 주소
        QSharedPointer<Cuma_Main> send_unit = nullptr;

        //bypass_index = bypass_limit_count - bypass_count
        int next_bypass_index = (o["bypass_limit_count"].toInt() - o["bypass_count"].toInt() - 1);

        send_unit_id = bypass_chain.at(next_bypass_index).toInt();
        Cuma_Debug("send bypass reply to unit : " + QString::number(send_unit_id));

        if(send_unit_id == m_Pid && next_bypass_index == -1)
        {
            Cuma_Debug("Bypass reply is mine !!", __LINE__);
            return 1;
        }

        o["bypass_count"] = o["bypass_count"].toInt() + 1;

        //바이패스 대상의 포인터를 찾음
        Cuma_Debug("search bypass chain pointer", __LINE__);
        send_unit = f_find_unit_from_inside_timeout_unit(send_unit_id);

        //만약 바이패스 유닛들을 못찾았을때 exception을 던짐
        Cuma_Debug("check bypass unit is nullptr", __LINE__);
        if(send_unit == nullptr)
        {
            throw Cuma_Error("f_reply_over_bypass_limit : Can't find bypass unit in this unit list", __LINE__, m_Pid);
        }

        //프로토콜을 전송함
        Cuma_Debug("send protocol", __LINE__);
        emit send_unit->s_recv(o);

        return 0;
    }

    catch(Cuma_Error& e)
    {
        e.show_error_string();
        return -1;
    }
}

QSharedPointer<Cuma_Main> Cuma_Main::f_find_unit_from_inside_timeout_unit(uint32_t unit_id)
{
    for(QSharedPointer<Cuma_Main>& p: m_Cuma_unit_inside_timeout_unit_list)
    {
        if(p->mf_get_pid() == unit_id)
        {
            return p;
        }
    }
    return nullptr;
}

QSharedPointer<Cuma_Main> Cuma_Main::f_find_unit_from_Cuma_unit_list(uint32_t unit_id)
{
    for(QSharedPointer<Cuma_Main>& p: m_Cuma_unit_list)
    {
        if(p->mf_get_pid() == unit_id)
        {
            return p;
        }
    }
    return nullptr;
}
