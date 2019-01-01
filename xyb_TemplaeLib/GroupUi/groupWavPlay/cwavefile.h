#ifndef CWAVEFILE_H
#define CWAVEFILE_H

#include <vector>

#include <stdio.h>

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

typedef struct _wav_riff_t
{
    char id[5]; //"RIFF"
    int  size;
    char type[5];   //"WAVE"
}wav_riff_t;

typedef struct _wav_format_t
{
    char  id[5];    //"fmt"
    int   size;
    short compression_code;
    short channels;
    int   samples_per_sec;
    int   avg_bytes_per_sec;
    short block_align;
    short bits_per_simple;
}wav_format_t;

typedef struct _war_fact_t
{
    char id[5];
    int size;
}wav_fact_t;

typedef struct _war_data_t
{
    char id[5];
    int size;
}wav_data_t;


class CWaveFile
{
public:
    CWaveFile();

    void GetDatas(std::vector<int> & vecDatas);

    void setScalingFactor(int);

    int getSimple();

    int getBitsPerSimple();

    int getChannels();

    void printWaveFileInfo();

    int getIScalingFactor() const;

    void FileClose();
    void FileOpen(const char *);

private:
    int mstrncasecmp(char *s1, char * s2, register int n);


    bool RiffChunk();
    bool FormatChunk();

    void WavRewind();

    int readData(size_t size);
private:
    FILE * fp;
    wav_riff_t riff;
    wav_format_t format;
    wav_fact_t fact;
    wav_data_t data;
    int file_size;
    int data_offset;
    int data_size;
    int iScalingFactor;
};

#endif // CWAVEFILE_H
