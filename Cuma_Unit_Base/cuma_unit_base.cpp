#include "cuma_unit_base.h"

Cuma_Unit_Base::Cuma_Unit_Base(QObject *parent) : QObject(parent)
{
    qsrand( QTime::currentTime().msec());
}

Cuma_Unit_Base::~Cuma_Unit_Base()
{

}

void Cuma_Unit_Base::set_Cuma_Unit_Count(uint32_t count)
{
    unit_count = count;
}

void Cuma_Unit_Base::set_Cuma_Unit_Ping_Timeout(uint32_t timeout)
{
    unit_timeout = timeout;
}

int Cuma_Unit_Base::set_target_unit(uint32_t unit_pid)
{
    foreach (const QSharedPointer<Cuma_Main>& p, this->Cuma_unit_list) {
        if(p->mf_get_pid() == unit_pid)
        {
            target_Cuma_Unit = p;
            return 0;
        }
    }

    target_Cuma_Unit = nullptr;
    return -1;
}

int Cuma_Unit_Base::set_target_to_read_File(QString file_name, QByteArray& arr)
{
    QJsonObject command;
    command["filename"] = file_name;
    emit target_Cuma_Unit->s_start_command(command);
    target_Cuma_Unit->get_File_obj()->save_file( file_name, arr);
    target_Cuma_Unit->get_File_obj()->set_File_Name(file_name);
    if (target_Cuma_Unit->get_File_obj()->read_file() < 0) { Cuma_Error("Cuma_Unit read file is fail ", __LINE__).show_error_string(); return -1;}
    return 0;
}

uint32_t Cuma_Unit_Base::get_Cuma_Unit_Count()
{
    return unit_count;
}

uint32_t Cuma_Unit_Base::alloc_unit()
{
    uint32_t i = 0;

    for( i = 0; i < unit_count; i++)
    {
        Cuma_unit_list.append( QSharedPointer<Cuma_Main>::create());
    }

    return i;
}

uint32_t Cuma_Unit_Base::init_unit()
{
    QVector<uint32_t> unit_pid_list = make_pid(this->unit_count);
    QVector<QVector<uint32_t>> unit_ping_limit_array = make_Unit_Ping_Limit(this->unit_count, this->unit_timeout);
    QMap<uint32_t, uint32_t> unit_ping_timeout_map = distribute_Ping_Timeout_Time(this->unit_count, this->unit_timeout/30, false);

    for(uint32_t i = 0; i < this->unit_count ; i++)
    {
        Cuma_unit_list[i]->mf_set_pid(unit_pid_list[i]);
        Cuma_unit_list[i]->mf_set_dealy_lst(unit_ping_limit_array);
        Cuma_unit_list[i]->mf_set_ping_limit_time(unit_ping_timeout_map[i] );
        Cuma_unit_list[i]->mf_set_unit_list(Cuma_unit_list);
    }
    return 0;
}

uint32_t Cuma_Unit_Base::init_unit(bool time_out_limit)
{
    QVector<uint32_t> unit_pid_list = make_pid(this->unit_count);
    QVector<QVector<uint32_t>> unit_ping_limit_array = make_Unit_Ping_Limit(this->unit_count, this->unit_timeout);
    QMap<uint32_t, uint32_t> unit_ping_timeout_map = distribute_Ping_Timeout_Time(this->unit_count, this->unit_timeout/30, false, time_out_limit);

    for(uint32_t i = 0; i < this->unit_count ; i++)
    {
        Cuma_unit_list[i]->mf_set_pid(unit_pid_list[i]);
        Cuma_unit_list[i]->mf_set_dealy_lst(unit_ping_limit_array);
        Cuma_unit_list[i]->mf_set_ping_limit_time(unit_ping_timeout_map[i] );
        Cuma_unit_list[i]->mf_set_unit_list(Cuma_unit_list);
    }
    return 0;
}

QVector<uint32_t> Cuma_Unit_Base::make_pid(uint32_t count)
{

    QVector<uint32_t> ret_vec;

    if(count == 0)
        return ret_vec;

    ret_vec.resize(count);

    for(uint32_t i = 0; i < count ; i++)
    {
        ret_vec.replace(i , i);
    }

    return ret_vec;
}

QVector<QVector<uint32_t> > Cuma_Unit_Base::make_Unit_Ping_Limit(uint32_t count, uint32_t limit)
{
    QVector<QVector<uint32_t> > ret_vec;

    if(count == 0)
        return ret_vec;

    ret_vec.resize(count);

    for(QVector<uint32_t>& u : ret_vec)
    {
        u.resize(count);
    }

    for (uint i = 0; i < count; i++)
    {
        for (uint j = 0; j < count; j++)
        {
            if(i == j)
            {
                ret_vec[i][j] = 0;
                break;
            }

            uint32_t ping_timeout = (qrand() % limit);
            ret_vec[i][j] = ping_timeout;
            ret_vec[j][i] = ping_timeout;
        }
    }
    return ret_vec;
}

QMap<uint32_t, uint32_t> Cuma_Unit_Base::distribute_Ping_Timeout_Time(uint32_t pid_last_count, uint32_t timegap, bool is_disturibute)
{
    //유닛들의 ping timeout의 타임 갭을 넣어서 분활 시간을 통해서 모든유닛들에게 균등/랜덤 으로 타임아웃시간을 할당함

    uint32_t gap_count = unit_timeout/timegap;

    if(unit_timeout % timegap != 0)
    {
        gap_count += 1;
    }

    uint32_t gap_unit_count = pid_last_count / gap_count;

    if(pid_last_count % gap_count != 0)
    {
        gap_unit_count += 1;
    }

    QMap<uint32_t, uint32_t> ret_map;


    if(is_disturibute == true)
    {
        uint32_t gap_count_temp = 0;
        for(uint i = 0; i < pid_last_count; i++)
        {

            if(i % gap_unit_count == 0 && i != 0)
            {
                gap_count_temp ++;
                ret_map[i] = gap_count_temp * timegap;
            }
            else
            {
                ret_map[i] = gap_count_temp * timegap;
            }
        }
    }
    else
    {
        for(uint i = 0; i < pid_last_count; i++)
        {
            ret_map[i] = (rand() % gap_count) * timegap;
        }
    }

    return ret_map;
}

QMap<uint32_t, uint32_t> Cuma_Unit_Base::distribute_Ping_Timeout_Time(uint32_t pid_last_count, uint32_t timegap, bool is_distribitue, bool set_timeout)
{
    QMap<uint32_t, uint32_t> ret_map;

    if(set_timeout == false)
    {
        return distribute_Ping_Timeout_Time( pid_last_count,  timegap,  is_distribitue);
    }
    else
    {
        for(uint i = 0; i < pid_last_count; i++)
        {
            ret_map[i] = unit_timeout;
        }
    }

    return ret_map;
}

int Cuma_Unit_Base::Attach_Cuma_Unit_to_Thread()
{
    for(uint i = 0; i < this->unit_count ; i++)
    {
        Cuma_Debug("unit count : " + QString::number(i));
        QSharedPointer<QThread> temp_thread = QSharedPointer<QThread>::create();
        this->Cuma_unit_list[i]->moveToThread(temp_thread.data());

        connect(temp_thread.data(), SIGNAL(finished()), temp_thread.data(), SLOT(quit()));
        connect(temp_thread.data(), SIGNAL(started()), this->Cuma_unit_list[i].data(), SLOT(sl_start_idle()));
        temp_thread.data()->wait();

        this->Cuma_Thread_list.append(temp_thread);

    }

    return 0;
}

int Cuma_Unit_Base::init_Cuma_Unit(uint32_t count)
{
    this->set_Cuma_Unit_Count( count);
    if ( this->alloc_unit() < 0)
    {
        Cuma_Error("alloc_unit() Cuma_Unit lists is fail", __LINE__);
        return -1;
    }

    else if(this->init_unit() < 0)
    {
        Cuma_Error("init_unit() Cuma_Unit lists is fail", __LINE__);
        return -1;
    }

    if(this->Attach_Cuma_Unit_to_Thread() < 0)
    {
        Cuma_Error("Attach_Cuma_Unit_to_Thread() Cuma_Unit lists is fail", __LINE__);
        return -1;
    }
    return 0;
}

int Cuma_Unit_Base::init_Cuma_Unit(uint32_t count, bool limit_timeout)
{
    this->set_Cuma_Unit_Count( count);
    if ( this->alloc_unit() < 0)
    {
        Cuma_Error("alloc_unit() Cuma_Unit lists is fail", __LINE__);
        return -1;
    }

    else if(this->init_unit(limit_timeout) < 0)
    {
        Cuma_Error("init_unit() Cuma_Unit lists is fail", __LINE__);
        return -1;
    }

    if(this->Attach_Cuma_Unit_to_Thread() < 0)
    {
        Cuma_Error("Attach_Cuma_Unit_to_Thread() Cuma_Unit lists is fail", __LINE__);
        return -1;
    }
    return 0;
}

void Cuma_Unit_Base::init_Cuma_Unit_File_Frag_dir()
{
    try{

        if(this->Cuma_unit_list.isEmpty())
        {
            throw Cuma_Error("Cuma_unit_list is empty", __LINE__);
        }

        for(QSharedPointer<Cuma_Main> m: this->Cuma_unit_list)
        {
            QDir unit_dir;
            QString temp_root_dir_path = QDir::currentPath();
            unit_dir.mkdir("unit" + QString::number(m->mf_get_pid()));
            unit_dir.cd("unit" + QString::number(m->mf_get_pid()));

            QDir::setCurrent(unit_dir.absolutePath());
            m->get_File_obj()->init_dir();
            QDir::setCurrent(temp_root_dir_path);

        }

    }catch(Cuma_Error& e)
    {
        e.show_error_string();
    }
}

void Cuma_Unit_Base::remove_all_unit_file()
{
    if(this->Cuma_unit_list.isEmpty() == true)
    {
        Cuma_Error("Cuma_Unit_list is empty please alloc & init unit", __LINE__).show_error_string();
        return;
    }

    for(QSharedPointer<Cuma_Main > m: this->Cuma_unit_list)
    {
        Cuma_Debug(" Unit pid : " + QString::number(m->mf_get_pid()), __LINE__);
        Cuma_Debug("path : " + m->get_File_obj()->get_File_Directory().absolutePath(), __LINE__);

        QDir unit_file_frag_dir = m->get_File_obj()->get_File_Directory();
        QStringList directory_list = unit_file_frag_dir.entryList();
        if(directory_list.isEmpty())
        {
            Cuma_Debug("this unit have no file inside directory ", __LINE__);
            return;
        }
        Cuma_Debug(directory_list.join(","), __LINE__);

        for( QString f_name: directory_list)
        {
            Cuma_Debug("search remove file_name :" + f_name, __LINE__);

            if(f_name == "." || f_name == "..")
            {
                Cuma_Debug("file_name is same continue: " + f_name, __LINE__);
                continue;
            }
            else
            {
                Cuma_Debug("remove file: " + f_name, __LINE__);
                if (unit_file_frag_dir.remove(f_name) == false)
                {
                    Cuma_Error("can't remove this File : " + f_name, __LINE__).show_error_string();
                    continue;
                }
            }
        }

        m->get_File_obj()->get_File_Directory().refresh();
    }
}

int Cuma_Unit_Base::start_unit_ping_spread()
{
    if(this->Cuma_Thread_list.empty())
    {
        Cuma_Error("Cuma_Thread_list is not set", __LINE__).show_error_string();
        return -1;
    }
    else if(this->Cuma_unit_list.empty())
    {
        Cuma_Error("Cuma_Thread_list is not set", __LINE__).show_error_string();
        return -1;
    }
    else if(this->target_Cuma_Unit == nullptr)
    {
        Cuma_Error("target_Cuma_Unit is empty", __LINE__).show_error_string();
        return -1;
    }

    for(QSharedPointer<QThread> thr_it: this->get_Cuma_Unit_Thread())
    {
        thr_it->start();
        if(thr_it->isRunning() == false)
        {
            Cuma_Error("Thread is not running", __LINE__).show_error_string();
        }
    }

    QJsonObject command;
    command["command"] = "ping";

    emit target_Cuma_Unit->s_start_command(command);

    QThread::sleep(2);

    Cuma_Debug("Emit is successful", __LINE__);
    return 0;
}

int Cuma_Unit_Base::start_all_unit_ping_spread()
{
    if(this->Cuma_Thread_list.empty())
    {
        Cuma_Error("Cuma_Thread_list is not set", __LINE__).show_error_string();
        return -1;
    }
    else if(this->Cuma_unit_list.empty())
    {
        Cuma_Error("Cuma_Thread_list is not set", __LINE__).show_error_string();
        return -1;
    }
    else if(this->target_Cuma_Unit == nullptr)
    {
        Cuma_Error("target_Cuma_Unit is empty", __LINE__).show_error_string();
        return -1;
    }

    for(QSharedPointer<QThread> thr_it: this->get_Cuma_Unit_Thread())
    {
        if(thr_it->isRunning() == false)
        {
            thr_it->start();

            if(thr_it->isRunning() == false)
            {
                Cuma_Error("Thread is not running", __LINE__).show_error_string();
            }

        }
    }

    QJsonObject command;
    command["command"] = "ping";

    for(QSharedPointer<Cuma_Main> unit: this->get_alloc_unit())
    emit unit->s_start_command(command);

    QThread::sleep(2);

    Cuma_Debug("Emit is successful", __LINE__);
    return 0;
}

int Cuma_Unit_Base::start_unit_file_binary_spread()
{
    if(this->Cuma_Thread_list.empty())
    {
        Cuma_Error("Cuma_Thread_list is not set", __LINE__).show_error_string();
        return -1;
    }
    else if(this->Cuma_unit_list.empty())
    {
        Cuma_Error("Cuma_Thread_list is not set", __LINE__).show_error_string();
        return -1;
    }
    else if(this->target_Cuma_Unit == nullptr)
    {
        Cuma_Error("target_Cuma_Unit is empty", __LINE__).show_error_string();
        return -1;
    }

    for(QSharedPointer<QThread> thr_it: this->get_Cuma_Unit_Thread())
    {
        if(thr_it->isRunning() == false)
        {
            thr_it->start();
            if(thr_it->isRunning() == false)
            {
                Cuma_Error("Thread is not running", __LINE__).show_error_string();
            }
        }
    }

    QJsonObject command;
    command["command"] = "upload";
    emit target_Cuma_Unit->s_start_command(command);

    QThread::sleep(2);

    Cuma_Debug("Emit is successful", __LINE__);
    return 0;
}

int Cuma_Unit_Base::start_unit_file_binary_request()
{
    if(this->Cuma_Thread_list.empty())
    {
        Cuma_Error("Cuma_Thread_list is not set", __LINE__).show_error_string();
        return -1;
    }
    else if(this->Cuma_unit_list.empty())
    {
        Cuma_Error("Cuma_Thread_list is not set", __LINE__).show_error_string();
        return -1;
    }
    else if(this->target_Cuma_Unit == nullptr)
    {
        Cuma_Error("target_Cuma_Unit is empty", __LINE__).show_error_string();
        return -1;
    }

    for(QSharedPointer<QThread> thr_it: this->get_Cuma_Unit_Thread())
    {
        if(thr_it->isRunning() == false)
        {
            thr_it->start();
            if(thr_it->isRunning() == false)
            {
                Cuma_Error("Thread is not running", __LINE__).show_error_string();
            }
        }
    }

    QJsonObject command;
    command["command"] = "download";
    emit target_Cuma_Unit->s_start_command(command);

    QThread::sleep(2);

    Cuma_Debug("Emit is successful", __LINE__);
    return 0;
}

int Cuma_Unit_Base::start_unit_file_binary_exsist()
{
    if(this->Cuma_Thread_list.empty())
    {
        Cuma_Error("Cuma_Thread_list is not set", __LINE__).show_error_string();
        return -1;
    }
    else if(this->Cuma_unit_list.empty())
    {
        Cuma_Error("Cuma_Thread_list is not set", __LINE__).show_error_string();
        return -1;
    }
    else if(this->target_Cuma_Unit == nullptr)
    {
        Cuma_Error("target_Cuma_Unit is empty", __LINE__).show_error_string();
        return -1;
    }

    for(QSharedPointer<QThread> thr_it: this->get_Cuma_Unit_Thread())
    {
        if(thr_it->isRunning() == false)
        {
            thr_it->start();
            if(thr_it->isRunning() == false)
            {
                Cuma_Error("Thread is not running", __LINE__).show_error_string();
            }
        }
    }

    QJsonObject command;
    command["command"] = "file_name";
    command["filename"] = "test.txt";
    emit target_Cuma_Unit->s_start_command(command);

    command.remove("command");
    command.remove("filename");

    command["command"] = "file_index";
    command["index"] = 0;
    emit target_Cuma_Unit->s_start_command(command);

    command.remove("command");
    command.remove("index");

    command["command"] = "filecheck";
    emit target_Cuma_Unit->s_start_command(command);
    QThread::sleep(2);

    Cuma_Debug("Emit is successful", __LINE__);
    return 0;
}

int Cuma_Unit_Base::start_unit_bypass()
{
    if(this->Cuma_Thread_list.empty())
    {
        Cuma_Error("Cuma_Thread_list is not set", __LINE__).show_error_string();
        return -1;
    }
    else if(this->Cuma_unit_list.empty())
    {
        Cuma_Error("Cuma_Thread_list is not set", __LINE__).show_error_string();
        return -1;
    }
    else if(this->target_Cuma_Unit == nullptr)
    {
        Cuma_Error("target_Cuma_Unit is empty", __LINE__).show_error_string();
        return -1;
    }

    for(QSharedPointer<QThread> thr_it: this->get_Cuma_Unit_Thread())
    {
        if(thr_it->isRunning() == false)
        {
            thr_it->start();
            if(thr_it->isRunning() == false)
            {
                Cuma_Error("Thread is not running", __LINE__).show_error_string();
            }
        }
    }

    QJsonObject command;
    command["command"] = "bypass";

    emit target_Cuma_Unit->s_start_command(command);

    return 0;
}

void Cuma_Unit_Base::finish_thread()
{
    for(QSharedPointer<QThread> t :Cuma_Thread_list)
    {
        t->quit();

        if(!t->wait(3000)) //Wait until it actually has terminated (max. 3 sec)
        {
            t->terminate(); //Thread didn't exit in time, probably deadlocked, terminate it!
            t->wait(); //We have to wait again here!
        }
    }
}

QVector<QSharedPointer<Cuma_Main> > Cuma_Unit_Base::get_alloc_unit()
{
    return Cuma_unit_list;
}

QVector<QSharedPointer<QThread> > Cuma_Unit_Base::get_Cuma_Unit_Thread()
{
    return Cuma_Thread_list;
}

QSharedPointer<Cuma_Main> Cuma_Unit_Base::get_target_unit()
{
    return this->target_Cuma_Unit;
}
