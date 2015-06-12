/*****************************************************************************

    (c) 2013 Hobu, Inc. hobu.inc@gmail.com

    Author: Uday Verma uday dot karan at gmail dot com

    This is free software; you can redistribute and/or modify it under the
    terms of the GNU Lesser General Licence as published by the Free Software
    Foundation. See the COPYING file for more information.

    This software is distributed WITHOUT ANY WARRANTY and without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************************/



#pragma once

#include <boost/noncopyable.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string.hpp>

#include <algorithm>
#include <stdexcept>
#include <points2grid/export.hpp>



class P2G_DLL las_file : public boost::noncopyable {
public:
    las_file() : is_open_(false) {
    }

    ~las_file() {
        close();
    }

    void open(const std::string& filename, int offset = 0, int count = -1) {
        using namespace boost::interprocess;

        start_offset_ = offset;
        count_ = count;

        pmapping_.reset(new file_mapping(filename.c_str(), read_only));
        pregion_.reset(new mapped_region(*pmapping_, read_only));

        void *addr = pregion_->get_address();

        std::string magic((char *)addr, (char *)addr + 4);
        if (!boost::iequals(magic, "LASF")) {
            throw std::runtime_error("Not a las file");
        }

        char versionMajor = readAs<char>(24);
        char versionMinor = readAs<char>(25);

        if ((int)(versionMajor * 10 + versionMinor) >= 13) {
            throw std::runtime_error("Only version 1.0-1.2 files are supported");
        }

        points_offset_ = readAs<unsigned int>(32*3);
        points_format_id_ = readAs<unsigned char>(32*3 + 8);
        points_struct_size_ = readAs<unsigned short>(32*3 + 8 + 1);
        points_count_ = readAs<unsigned int>(32*3 + 11);

        // std::cerr << "points count: " << points_count_ << std::endl;

        size_t start = 32*3 + 35;
        readN(start, scale_, 3); start += sizeof(double) * 3;
        readN(start, offset_, 3); start += sizeof(double) * 3;

        maxs_[0] = readAs<double>(start); mins_[0] = readAs<double>(start + sizeof(double)); start += 2*sizeof(double);
        maxs_[1] = readAs<double>(start); mins_[1] = readAs<double>(start + sizeof(double)); start += 2*sizeof(double);
        maxs_[2] = readAs<double>(start); mins_[2] = readAs<double>(start + sizeof(double)); start += 2*sizeof(double);

        // std::cerr << "region size: " << pregion_->get_size() << std::endl;
        // std::cerr << "points offset: " << points_offset_ << std::endl;


        uint64_t diff = pregion_->get_size() - points_offset_;

        if (diff % stride() != 0)
            throw std::runtime_error("Point record data size is inconsistent");

        if (diff / stride() != points_count_)
            throw std::runtime_error("Point record count is inconsistent with computed point records size");

        updateMinsMaxes();

        is_open_ = true;
    }

    size_t size() {
        return pregion_->get_size();
    }

    void *points_offset() {
        return (char *)pregion_->get_address() + points_offset_ + start_offset_;
    }

    void close() {
        pregion_.reset();
        pmapping_.reset();
        is_open_ = false;
    }

    size_t stride() {
        if (points_struct_size_ != 0)
            return points_struct_size_;

        switch(points_format_id_) {
            case 0:
                return 20;
            case 1:
                return 28;
            case 2:
                return 26;
            case 3:
                return 34;
            case 4:
                return 57;
            case 5:
                return 63;
            default:
                break;
        }
        throw std::runtime_error("Unknown point format");
    }

    size_t points_count() const {
        if (count_ == -1)
            return points_count_;

        return std::min(points_count_, (unsigned int)count_);
    }

    double* minimums() { return mins_; }
    double* maximums() { return maxs_; }

    double *scale() { return scale_; }
    double *offset() { return offset_; }

    bool is_open() { return is_open_; }

    inline double getX(size_t point)
    {
        char *position = (char*)points_offset() + stride()* point;

        int *xi = (int *)position;

        double x = *xi * scale_[0] + offset_[0];

        return x;
    }

    inline double getY(size_t point)
    {
        char *position = (char *)points_offset() + stride() * point + sizeof(int);

        int *yi = (int *)position;

        double y = *yi * scale_[1] + offset_[1];

        return y;
    }

    inline double getZ(size_t point)
    {
        char *position = (char *)points_offset() + stride() * point + sizeof(int) + sizeof(int);

        int *zi = (int *)position;

        double z = *zi * scale_[2] + offset_[2];

        return z;
    }

    inline int getClassification(size_t point)
    {
        int classification_offset = 15;
        char *position = (char *)points_offset() + stride() * point + classification_offset;

        int *classi = (int *)position;

        int classification = *classi & 0x1F;

        return classification;
    }

private:
    void updateMinsMaxes() {
        if (start_offset_ == 0 && count_ == -1)
            return; // no update required if no subrange is requested

        int largest = std::numeric_limits<int>::max();
        int smallest = std::numeric_limits<int>::min();

        int n[3] = { largest, largest, largest };
        int x[3] = { smallest, smallest, smallest };

        char *ip = (char *)points_offset();
        for (size_t i = 0 ; i < points_count() ; i ++) {
            int *p = (int *)ip;
            for (int j = 0 ; j < 3 ; j ++) {
                n[j] = std::min(n[j], p[j]);
                x[j] = std::max(x[j], p[j]);
            }

            ip += stride();
        }

        for (int i = 0 ; i < 3 ; i++) {
            mins_[i] = n[i] * scale_[i] + offset_[i];
            maxs_[i] = x[i] * scale_[i] + offset_[i];
        }
    }

    template<typename T>
    T readAs(size_t offset) {
        return *((T*)((char*)pregion_->get_address() + offset));
    }

    template<typename T>
    void readN(size_t offset, T* dest, size_t n) {
        char *buf = (char *)pregion_->get_address() + offset;
        for(size_t i = 0 ; i < n ; i ++) {
            dest[i] = *((T*)(buf + sizeof(T) * i));
        }
    }

private:
    boost::shared_ptr<boost::interprocess::file_mapping> pmapping_;
    boost::shared_ptr<boost::interprocess::mapped_region> pregion_;

    unsigned int start_offset_;
    int count_;
    unsigned int points_offset_;
    unsigned char points_format_id_;
    unsigned int points_count_;
    unsigned short points_struct_size_;

    double scale_[3], offset_[3], mins_[3], maxs_[3];

    bool is_open_;
};

