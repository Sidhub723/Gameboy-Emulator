#include "rw_log.h"

RWLog* RWLog::get_instance()
{
    if(instance == nullptr)
    {
        instance = new RWLog();
    }
    return instance;
}

uint32_t RWLog::get_read_count()
{
    return readCount;
}

uint32_t RWLog::get_write_count()
{
    return writeCount;
}

void RWLog::log_read16(uint32_t addr)
{
    RWLogEntry entry(true, addr, 0);
    log.push_back(entry);
    readCount++;
}

void RWLog::log_write16(uint32_t addr, uint16_t data)
{
    RWLogEntry entry(false, addr, data);
    log.push_back(entry);
    writeCount++;
}

void RWLog::log_read8(uint32_t addr)
{
    RWLogEntry entry(true, addr, 0);
    log.push_back(entry);
    readCount++;
}

void RWLog::log_write8(uint32_t addr, uint8_t data)
{
    RWLogEntry entry(false, addr, data);
    log.push_back(entry);
    writeCount++;
}

void RWLog::clear()
{
    log.clear();
}

RWLogEntry RWLog::get_entry(int index)
{
    return log[index];
}

int RWLog::size()
{
    return log.size();
}