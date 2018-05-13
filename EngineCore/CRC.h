#pragma once

class hash
{
public:


	static UINT32 calculate_crc32c(UINT32 crc32c, const unsigned char * buffer, unsigned int length);

private:
	static UINT32 singletable_crc32c(UINT32 crc, const void * buf, size_t size);
	static UINT32 crc32c_sb8_64_bit(UINT32 crc, const unsigned char * p_buf, UINT32 length, UINT32 init_bytes);
	static UINT32 multitable_crc32c(UINT32 crc32c, const unsigned char * buffer, unsigned int length);
};

// outside for time being

