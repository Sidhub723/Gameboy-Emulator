#ifndef RW_LOG_H
#define RW_LOG_H

#include <cstdint>
#include <vector>

struct RWLogEntry
{
    RWLogEntry(bool is_read, uint32_t addr, uint32_t data)
    {
        this->is_read = is_read;
        this->addr = addr;
        this->data = data;
    }

    bool is_read;
    uint32_t addr;
    uint32_t data;
};

class RWLog
{
public:
    static RWLog* get_instance();

    uint32_t get_read_count();;
    uint32_t get_write_count();

    void log_read16(uint32_t addr);
    void log_write16(uint32_t addr, uint16_t data);
    void log_read8(uint32_t addr);
    void log_write8(uint32_t addr, uint8_t data);
    
    void clear();
    RWLogEntry get_entry(int index);
    int size();

private:
    RWLog()
    {
        readCount = 0;
        writeCount = 0;
    }

private:
    std::vector<RWLogEntry> log;
    uint32_t readCount;
    uint32_t writeCount;
    inline static RWLog* instance;
};

#endif