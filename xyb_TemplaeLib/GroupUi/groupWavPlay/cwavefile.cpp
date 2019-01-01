#include "cwavefile.h"

#include <string.h>
#include <stdlib.h>
#include <wchar.h>
#include <ctype.h>
#include <QDebug>

CWaveFile::CWaveFile():
    fp(0),
    riff({0}),
    format({0}),
    fact({0}),
    data({0}),
    file_size(0),
    data_offset(0),
    data_size(0),
    iScalingFactor(1)
{
    //FileOpen(cFileName);
}

void CWaveFile::WavRewind()
{
    if(fseek(fp, data_offset, SEEK_SET) < 0)
    {
        printf("wav rewind failedly\n");
    }
}

int CWaveFile::readData(size_t size)
{
    if(size > 4 || size <= 0 || size == 3)
    {
        return -1;
    }

    char buffer[5] = {0};

    fread(buffer, 1, size, fp);
    switch(size)
    {
    case 1:
        return (*buffer);
    case 2:
        return *((short *)(buffer));
    case 4:
        return *((int *)(buffer));
    default:
        return -1;
    }
}

int CWaveFile::getIScalingFactor() const
{
    return iScalingFactor;
}

void CWaveFile::GetDatas(std::vector<int> &vecDatas)
{
    ushort usBytesPerSample = format.bits_per_simple * 0.125;
    if(usBytesPerSample == 0
            || iScalingFactor <= 0
            || NULL == fp
            )
    {
        return;
    }

    int iNum = data_size / usBytesPerSample;

    WavRewind();
    if(format.channels == 1)
    {
        if(usBytesPerSample == 1)
        {
            for(int i = 0; i < iNum; i++)
            {
                if(i % iScalingFactor != 0)
                {
                    fseek(fp, 1, SEEK_CUR);
                    continue;
                }
                char cData = 0;
                fread(&cData, 1, 1, fp);
                vecDatas.push_back(cData);
            }
        }
        else if(usBytesPerSample == 2)
        {
            for(int i = 0; i < iNum; i++)
            {
                if(i % iScalingFactor != 0)
                {
                    fseek(fp, 2, SEEK_CUR);
                    continue;
                }

                short sData = 0;
                fread((char *)(&sData), 1, 2, fp);
                //qDebug() << sData;
                vecDatas.push_back(sData);
            }
        }
        else if(usBytesPerSample == 4)
        {
            for(int i = 0; i < iNum; i++)
            {
                if(i % iScalingFactor != 0)
                {
                    fseek(fp, 4, SEEK_CUR);
                    continue;
                }
                int iData = 0;
                fread((char *)(&iData), 1, 4, fp);
                vecDatas.push_back(iData);
            }
        }
        else if(usBytesPerSample == 3)
        {
            for(int i = 0; i < iNum; i++)
            {
                if(i % iScalingFactor != 0)
                {
                    fseek(fp, 3, SEEK_CUR);
                    continue;
                }
                char cData = 0;
                short sData = 0;
                fread(&cData, 1, 1, fp);
                fread((char *)(&sData), 1, 2, fp);

                vecDatas.push_back(cData + sData);
            }
        }
    }
    else if(format.channels == 2)
    {
        iNum *= 0.5;
        if(usBytesPerSample == 1)
        {
            for(int i = 0; i < iNum; i++)
            {
                if(i % iScalingFactor != 0)
                {
                    fseek(fp, 2, SEEK_CUR);
                    continue;
                }
                char cData = 0;
                fread(&cData, 1, 1, fp);
                vecDatas.push_back(cData);
                fseek(fp, 1, SEEK_CUR);
            }
        }
        else if(usBytesPerSample == 2)
        {
            for(int i = 0; i < iNum; i++)
            {
                if(i % iScalingFactor != 0)
                {
                    fseek(fp, 4, SEEK_CUR);
                    continue;
                }
                short sData = 0;
                fread((char *)(&sData), 1, 2, fp);
                vecDatas.push_back(sData);
                fseek(fp, 2, SEEK_CUR);
            }
        }
        else if(usBytesPerSample == 4)
        {
            for(int i = 0; i < iNum; i++)
            {
                if(i % iScalingFactor != 0)
                {
                    fseek(fp, 8, SEEK_CUR);
                    continue;
                }
                int iData = 0;
                fread((char *)(&iData), 1, 4, fp);
                vecDatas.push_back(iData);
                fseek(fp, 4, SEEK_CUR);
            }
        }
        else if(usBytesPerSample == 3)
        {
            for(int i = 0; i < iNum; i++)
            {
                if(i % iScalingFactor != 0)
                {
                    fseek(fp, 6, SEEK_CUR);
                    continue;
                }
                char cData = 0;
                short sData = 0;
                fread(&cData, 1, 1, fp);
                fread((char *)(&sData), 1, 2, fp);

                vecDatas.push_back(cData + sData);
                fseek(fp, 3, SEEK_CUR);
            }
        }
    }
    return;
}

void CWaveFile::setScalingFactor(int iScalingFactor)
{
    if(iScalingFactor <= 0)
    {
        this->iScalingFactor = 1;
        return;
    }
    this->iScalingFactor = iScalingFactor;
}

int CWaveFile::getSimple()
{
    return format.samples_per_sec/iScalingFactor;
}

int CWaveFile::getBitsPerSimple()
{
    return format.bits_per_simple;
}

int CWaveFile::getChannels()
{
    return format.channels;
}

void CWaveFile::printWaveFileInfo()
{
    qDebug() << "riff.id:" << riff.id;
    qDebug() << "riff.size:" << riff.size;
    qDebug() << "riff.type:" << riff.type;

    qDebug() << "format.avg_bytes_per_sec:" << format.avg_bytes_per_sec;
    qDebug() << "format.bits_per_simple:" << format.bits_per_simple;
    qDebug() << "format.block_align:" << format.block_align;
    qDebug() << "format.channels:" << format.channels;
    qDebug() << "format.compression_code:" << format.compression_code;
    qDebug() << "format.id:" << format.id;
    qDebug() << "format.samples_per_sec:" << format.samples_per_sec;
    qDebug() << "format.size:" << format.size;

    qDebug() << "fact.id:" << fact.id;
    qDebug() << "fact.size:" << fact.size;

    qDebug() << "data.id:" << data.id;
    qDebug() << "data.size:" << data.size;

    qDebug() << "data_size:" << data_size;
    qDebug() << "data_offset:" << data_offset;
    qDebug() << "file_size:" << file_size;
    qDebug() << "iScalingFactor:" << iScalingFactor;

}

void CWaveFile::FileOpen(const char *file_name)
{
    if(NULL == file_name)
    {
        qDebug()<<"file_name is NULL!";
        return;
    }

    char buffer[256] = {0};
    int read_len = 0;
    int offset = 0;

    if(NULL != fp)
    {
        qDebug()<<"fp not close!";
        fclose(fp);
    }
    fp = fopen(file_name, "rb");

    if(NULL == fp)
    {
        qDebug()<<"wav file open error!"<<errno;
        return;
    }

    if(!RiffChunk())
    {
        qDebug()<<"error wav file, riffChunk read error!";
        fclose(fp);
        return;
    }

    offset += 12;

    while(1)
    {
        char id_buffer[5] = {0};
        int tmp_size = 0;
        read_len = fread(buffer, 1, 8, fp);
        if(read_len < 8)
        {
            qDebug() <<"read_len err"<< read_len;
            fclose(fp);
            break;
        }

        memcpy(id_buffer, buffer, 4);
        tmp_size = *(int *)(buffer + 4);

        if(0 == mstrncasecmp((char *)"FMT", id_buffer, 3))
        {
            memcpy(format.id, id_buffer, 3);
            format.size = tmp_size;
            if(!FormatChunk())
            {
                qDebug() <<"FormatChunk return false";
                fclose(fp);
                break;
            }
        }
        else if(0 == mstrncasecmp((char *)"DATA", id_buffer, 4))
        {
           memcpy(data.id, id_buffer, 4);
           data.size = tmp_size;
           offset += 8;
           data_offset = offset;
           data_size = data.size;
           break;
        }
        else
        {
            qDebug()<<"unhandled chunk:"<<id_buffer<<"size: %d"<<tmp_size;
            fseek(fp, tmp_size, SEEK_CUR);
        }
        offset += 8 + tmp_size;
    }
    return;
}

void CWaveFile::FileClose()
{
    if(NULL != fp)
    {
        fclose(fp);
        fp = NULL;
    }
    return;
}

bool CWaveFile::FormatChunk()
{
    char buffer[256] = {0};
    int read_len = 0;

    read_len = fread(buffer, 1, format.size, fp);
    if(read_len < format.size)
    {
        return false;
    }
    format.compression_code = *(short *)buffer;
    format.channels = *(short *)(buffer+2);         //声道数
    format.samples_per_sec = *(int *)(buffer + 4);  //采样率
    format.avg_bytes_per_sec = *(int *)(buffer + 8);//平均字节率
    format.block_align = *(short *)(buffer + 12);   //数据块对齐
    format.bits_per_simple = *(short *)(buffer + 14);//样本位数
    return true;
}

bool CWaveFile::RiffChunk()
{
    char buffer[256] = {0};
    int read_len = 0;

    read_len = fread(buffer, 1, 12, fp);

    if(read_len < 12)
    {
        return false;
    }

    if(mstrncasecmp((char *)"RIFF", buffer, 4))
    {
        return false;
    }

    memcpy(riff.id, buffer, 4);
    riff.size = *(int *)(buffer + 4);

    if(mstrncasecmp((char *)"WAVE", buffer + 8, 4))
    {
        return false;
    }
    memcpy(riff.type, buffer + 8, 4);

    file_size = riff.size + 8;

    return true;
}

int CWaveFile::mstrncasecmp(char *s1, char *s2, int n)
{
    while(--n >= 0 && toupper((unsigned char)*s1) == toupper((unsigned char)*s2++))
        if(*s1++ == ' ')
            return 0;
    return (n < 0 ? 0 : toupper((unsigned char)*s1) - toupper((unsigned char)*--s2));
}
