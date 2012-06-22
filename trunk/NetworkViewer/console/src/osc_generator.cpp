#include <QDebug>
#include "ModuleConfiguration.h"
#include <QTextStream>
#include <QIODevice>

void write_variable_interface(const ModuleConfiguration &conf, QTextStream &out, unsigned int deviceID = 0)
{
    out<<"//This code is auto-generated. Do not modify.\n";
    out<<"int process_input(const char *data, int length, unsigned char deviceID)\n";
    out<<"{\n";

    /*
        Initialize message here...
    */
    out<<"\tNETV_MESSAGE message;\n";

    out<<"\tmessage.msg_priority=NETV_PRIORITY_HIGHEST;\n";
    out<<"\tmessage.msg_type=NETV_TYPE_REQUEST_DATA;\n";
    out<<"\tmessage.msg_eeprom_ram=NETV_REQUEST_RAM;\n";
    out<<"\tmessage.msg_read_write=NETV_REQUEST_WRITE;\n";
    out<<"\tmessage.msg_dest=deviceID;\n";
    out<<"\tmessage.msg_remote=0;\n";



    for (unsigned int i = 0; i < conf.size(); i++)
    {
        QString path= QString("/") + conf.getConfigName() + "/" + QString::number(deviceID) + "/" + conf[i]->getName();
        if (i == 0)
        {
            out << "\tif (strncmp(\"" << path <<"\""<< ", data, " << path.size() << ") == 0)\n";
        }
        else
        {
            out << "\telse if (strncmp(\"" << path <<"\""<< ", data, " << path.size() << ") == 0)\n";
        }

        out << "\t{\n";
        ModuleVariable *var = conf[i];

        out << "\t\tif(length < " << path.size() + 2 <<") return -1;\n";
        out << "\t\tchar format = data[" << path.size() + 2 << "];\n";
        out << "\t\tmessage.msg_cmd=" << var->getOffset() << ";\n";

        switch(var->getType())
        {
        case ModuleVariable::DOUBLE:
            out << "\t\tif (format != 'd' || length < " << path.size() + 11 <<") return -1;\n";
            out << "\t\tmessage.msg_data_length=8;\n";
            out << "\t\tmessage.msg_data[0]=data[" << path.size() + 11<<"];\n";
            out << "\t\tmessage.msg_data[1]=data[" << path.size() + 10<<"];\n";
            out << "\t\tmessage.msg_data[2]=data[" << path.size() + 9<<"];\n";
            out << "\t\tmessage.msg_data[3]=data[" << path.size() + 8<<"];\n";
            out << "\t\tmessage.msg_data[4]=data[" << path.size() + 7<<"];\n";
            out << "\t\tmessage.msg_data[5]=data[" << path.size() + 6<<"];\n";
            out << "\t\tmessage.msg_data[6]=data[" << path.size() + 5<<"];\n";
            out << "\t\tmessage.msg_data[7]=data[" << path.size() + 4<<"];\n";
            break;

        case ModuleVariable::FLOAT:
            out << "\t\tif (format != 'f' || length < " << path.size() + 7 <<") return -1;\n";
            out << "\t\tmessage.msg_data_length=4;\n";
            out << "\t\tmessage.msg_data[0]=data[" << path.size() + 7<<"];\n";
            out << "\t\tmessage.msg_data[1]=data[" << path.size() + 6<<"];\n";
            out << "\t\tmessage.msg_data[2]=data[" << path.size() + 5<<"];\n";
            out << "\t\tmessage.msg_data[3]=data[" << path.size() + 4<<"];\n";
            break;

        case ModuleVariable::SINT32:
            out << "\t\tif (format != 'i' || length < " << path.size() + 7 <<") return -1;\n";
            out << "\t\tmessage.msg_data_length=4;\n";
            out << "\t\tmessage.msg_data[0]=data[" << path.size() + 7<<"];\n";
            out << "\t\tmessage.msg_data[1]=data[" << path.size() + 6<<"];\n";
            out << "\t\tmessage.msg_data[2]=data[" << path.size() + 5<<"];\n";
            out << "\t\tmessage.msg_data[3]=data[" << path.size() + 4<<"];\n";
            break;

        case ModuleVariable::UINT32:
            out << "\t\tif (format != 'i' || length < " << path.size() + 7 <<") return -1;\n";
            out << "\t\tmessage.msg_data_length=4;\n";
            out << "\t\tmessage.msg_data[0]=data[" << path.size() + 7<<"];\n";
            out << "\t\tmessage.msg_data[1]=data[" << path.size() + 6<<"];\n";
            out << "\t\tmessage.msg_data[2]=data[" << path.size() + 5<<"];\n";
            out << "\t\tmessage.msg_data[3]=data[" << path.size() + 4<<"];\n";
            break;

        case ModuleVariable::SINT16:
            out << "\t\tif (format != 'i' || length < " << path.size() + 7 <<") return -1;\n";
            out << "\t\tmessage.msg_data_length=2;\n";
            out << "\t\tmessage.msg_data[0]=data[" << path.size() + 7<<"];\n";
            out << "\t\tmessage.msg_data[1]=data[" << path.size() + 6<<"];\n";
            break;

        case ModuleVariable::UINT16:
            out << "\t\tif (format != 'i' || length < " << path.size() + 7 <<") return -1;\n";
            out << "\t\tmessage.msg_data_length=2;\n";
            out << "\t\tmessage.msg_data[0]=data[" << path.size() + 7<<"];\n";
            out << "\t\tmessage.msg_data[1]=data[" << path.size() + 6<<"];\n";
            break;

        case ModuleVariable::SINT8:
            out << "\t\tif (format != 'i' || length < " << path.size() + 7 <<") return -1;\n";
            out << "\t\tmessage.msg_data_length=1;\n";
            out << "\t\tmessage.msg_data[0]=data[" << path.size() + 7<<"];\n";
            break;

        case ModuleVariable::UINT8:
            out << "\t\tif (format != 'i' || length < " << path.size() + 7 <<") return -1;\n";
            out << "\t\tmessage.msg_data_length=1;\n";
            out << "\t\tmessage.msg_data[0]=data[" << path.size() + 7<<"];\n";
            break;

        }

        out << "\t}\n";
    }

    out<<"\t//Sending message...\n";
    out<<"\treturn netv_send_message(&message);\n";
    out<<"}\n\n";
}


int main(int argc, char* argv[])
{
    ModuleConfiguration config;
    QTextStream qout(stdout);

    if (argc > 2)
    {
        qDebug("Processing : %s",argv[1]);
        if (config.loadConfiguration(argv[1],true));
        {
            qDebug("Configuration loaded : %s",argv[1]);
            write_variable_interface(config,qout,QString(argv[2]).toUInt());
        }

    }
    else
    {
        qDebug("Usage : %s <xml configuration file> <module_id>",argv[0]);
    }
	return 0;
}

