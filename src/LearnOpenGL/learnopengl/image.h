#pragma once

class image
{
public:
	static image& GetInstance();
	~image();

private:
	image();

public:
	unsigned char * load(const char* path, int& width, int& height, int& nrChannels, int req_comp = 0);
	void free(unsigned char * data);

};