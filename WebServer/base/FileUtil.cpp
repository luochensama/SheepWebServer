//
// Created by luochensama on 2022/3/3.
//

#include "FileUtil.h"

FileWriter::FileWriter(std::string filename) : fp_(fopen(filename.c_str(),"ae")){
    setbuffer(fp_,buffer_,sizeof buffer_);
}

FileWriter::~FileWriter() {
    fclose(fp_);
}

void FileWriter::append(const char *logline, size_t len) {
    size_t writen = this->write(logline,len);
    while(writen < len){
        size_t addlen = write(logline + writen,len-writen);
        if(!addlen){
            int err = ferror(fp_);
            if (err) fprintf(stderr, "AppendFile::append() failed !\n");
            break;
        }
        writen += addlen;
    }
}

void FileWriter::flush() {
    fflush(fp_);
}

size_t FileWriter::write(const char *logline, size_t len) {
    return fwrite_unlocked(logline,1,len,fp_);
}
