#include"utils.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>
#include<malloc.h>
#define logger(str) (std::cout<<"["<<str<<"]: ")
//#include<opencv2/opencv.hpp>

GLuint load_shader(const char* filename, GLenum type) {
	GLuint shader = glCreateShader(type);
	std::string code;
	std::ifstream file;
	file.open(filename);
	if (!file) {
		logger("Error") << "Can not open file" << filename << std::endl;
		return 0;
	}
	while (!file.eof()) {
		char word;
		word = file.get();
		code = code + word;
	}
	code.pop_back();
	const char* str = code.c_str();
	//logger("File") << str<<std::endl;
	//logger("Info") << "Code length" << code.length() << std::endl;
	const int length = code.length();
	glShaderSource(shader, 1, &str, &length);
	glCompileShader(shader);
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char errlog[512];
		glGetShaderInfoLog(shader, 512, NULL, errlog);
		logger("Error") << errlog << std::endl;
	}
	else {
		logger("Info") << "Shader Compiled." << std::endl;
	}
	logger("Code") << "\n" << code << std::endl;
	return shader;
}
GLuint CreateProgram(GLuint vertexShader, GLuint fragShader) {
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);
	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		logger("Error") << "Link Error." << std::endl;

	}
	else {
		logger("Info") << "Link Successful" << std::endl;
	}
	return program;
}

GLuint load_texture(const char* img_path) {
	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* imgdata = stbi_load(img_path, &width, &height, &channels, 0);
	//cv::Mat src;
	//src = cv::imread(img_path, 1);
	//cv::cvtColor(src, src, cv::COLOR_BGR2RGB);
	////unsigned char* cvdata = src.ptr<unsigned char>(0);
	//unsigned char* cvdata = src.data;
	unsigned char* data = (unsigned char*)malloc(sizeof(unsigned char) * width * height * 4);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			*(data + j * width * 4 + i * 4 + 0) = *(imgdata + j * width * channels + i * channels + 0);
			*(data + j * width * 4 + i * 4 + 1) = *(imgdata + j * width * channels + i * channels + 1);
			*(data + j * width * 4 + i * 4 + 2) = *(imgdata + j * width * channels + i * channels + 1);
			*(data + j * width * 4 + i * 4 + 3) = 1;
		}
	}
	logger("Info") << "Get texture:" << " width " << width <<
		" height " << height << " channel " << channels << " from " << img_path << std::endl;
	logger("Info") << "data buffer: from " << (unsigned int)data << " to "<< (unsigned int)(data + height * width) <<std::endl;
	GLuint texture;
	//glCreateTextures(GL_TEXTURE_2D, 1, &texture);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	//ÉèÖÃÊôÐÔ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	if (imgdata) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		logger("Info") << "Fail to load image" << std::endl;

	}
	free(data);
	stbi_image_free(imgdata);
	return texture;
}