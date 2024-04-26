#ifndef _VAS_IO_H_
#define _VAS_IO_H_

#include "base/vasdef.hpp"
#include <deque>
#include <fstream>
#include <string>

namespace vastina {

class Buffer {
  protected:
    std::deque<std::string> buffer_queue_;

  public:
    Buffer() : buffer_queue_{} {}
    ~Buffer() { buffer_queue_.clear(); }
    inline u32 PushBack(const std::string &str) {
        buffer_queue_.push_back(str);
        return buffer_queue_.size() - 1;
    }
    inline u32 PushBack(const string_view &str) {
        std::string s{str};
        return PushBack(s);
    }
    inline void Insert(u32 pos, const std::string &str) {
        buffer_queue_.insert(buffer_queue_.begin() + pos, str);
    }
    inline std::string PopBack() {
        auto str = buffer_queue_.at(buffer_queue_.size() - 1);
        buffer_queue_.pop_back();
        return str;
    }
    inline std::string PopFront() {
        auto str = buffer_queue_.at(0);
        buffer_queue_.pop_front();
        return str;
    }
};

class Writer : public Buffer {
  protected:
    std::string file_name_;
    std::fstream fs_; // file stream is not something nice

  public:
    Writer(const string_view &file_name) : Buffer(), file_name_(file_name), fs_{} {}
    ~Writer() {
        if (fs_.is_open())
            fs_.close();
        file_name_.clear();
    }
    inline void Open() { fs_.open(file_name_); }
    inline void Close() { fs_.close(); }
    inline void WriteLine() {
        auto flag{false};
        while (true) {
            for (auto &&ch : buffer_queue_.front()) {
                if ('\n' == ch) {
                    flag = true;
                    break;
                }
                fs_ << ch;
            }
            if (flag)
                break;
            buffer_queue_.pop_front();
        }
    }
    inline void WriteOne() { fs_ << PopFront(); }
    inline void WriteAll() {
        while (!buffer_queue_.empty()) {
            WriteOne();
        }
    }
};

} // namespace vastina

#endif