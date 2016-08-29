#ifndef __COMPRESS_H__
#define __COMPRESS_H__

#include"HuffmanTree.h"
#include<assert.h>
#include<string>

using namespace std;
typedef unsigned long LongType;

struct CharInfo
{
	unsigned char _ch;//字符
	LongType _count;//字符出现次数
	string _code;//哈弗曼编码

	CharInfo(LongType count = 0)
	:_count(count)
	{}
	
	bool operator>(const CharInfo&com)const
	{
		return _count > com._count;
	}
	bool operator<(const CharInfo&com)const
	{
		return _count < com._count;
	}
	bool operator!=(const CharInfo&com)const
	{
		return _count != com._count;
	}
	CharInfo operator+(const CharInfo&com)const
	{
		return CharInfo(_count + com._count);
	}

};

class FileCompress
{
public:
	FileCompress()
	{
		for (int i = 0; i < 256; i++)
		{
			_Info[i]._ch = i;
			_Info[i]._count = 0;
		}
	}
public:
	//压缩
	void Compress(const char*file)
	{
		//打开文件
		FILE*fOut = fopen(file, "rb");
		assert(fOut);
		LongType ChCount = 0;
		//统计字符出现次数
		char ch = fgetc(fOut);
		while (ch != EOF)
		{
			++ChCount;
			_Info[(unsigned char)ch]._count++;
			ch = fgetc(fOut);
		}
		//重建哈弗曼树
		CharInfo invalid(0);//定义非法值，过滤未出现的字符
		HuffmanTree<CharInfo>ht(_Info, 256, invalid);
		//生成哈弗曼编码
		string code;
		_CreateHuffCode(ht.GetRoot(), code);
		//压缩后的文件名
		string comfile = file;
		comfile += ".compress";
		FILE* fIn = fopen(comfile.c_str(), "wb");
		assert(fIn);

		//从文件首字符进行压缩
		fseek(fOut, 0, SEEK_SET);
		unsigned char value = 0;
		size_t index = 0;
		ch = fgetc(fOut);
		while (ch != EOF)
		{
			string& code = _Info[(unsigned char)ch]._code;
			for (size_t i = 0; i < code.size(); i++)
			{
				if (code[i] == '1')
				{
					value |= 1;
				}
				//将8位满的写入压缩文件
				if (++index == 8)
				{
					fputc(value, fIn);
					value = 0;
					index = 0;
				}
				value <<= 1;
			}
			ch = fgetc(fOut);
		}
		//处理没有满8位的，后面补0
		if (index != 0)
		{
			value <<= (7 - index);
			fputc(value, fIn);
		}
		//配置文件
		string config = file;
		config += ".config";
		FILE*fConfig = fopen(config.c_str(), "wb");
		assert(fConfig);
		
		//写入字符总个数
		char buffer[128];
		itoa(ChCount, buffer, 10);
		fputs(buffer, fConfig);
		fputc('\n', fConfig);
		//写入：字符，次数
		string line;
		for (int i = 0; i < 256; ++i)
		{
			if (_Info[i]._count>0)
			{
				//line格式控制
				line += _Info[i]._ch;
				line += ',';
				itoa(_Info[i]._count, buffer, 10);
				line += buffer;
				line += '\n';

				fputs(line.c_str(), fConfig);//写入配置文件
			}
			line.clear();//清空line
		}
		fclose(fOut);
		fclose(fIn);
		fclose(fConfig);
	}

	void UnConpress(const char*file)
	{
		//读配置文件，解压缩
		string config = file;
		config += ".config";
		FILE*fConfig = fopen(config.c_str(), "r");
		assert(fConfig);
		//读取源文件字符总个数 
		string line;
		LongType ChCount = 0;

		if (ReadLine(fConfig, line))
		{
			ChCount += atoi(line.c_str());
			line.clear();
			if (ChCount == 0)
				return;
		}
		else
			return;

		while (ReadLine(fConfig, line))
		{
			if (line.empty())//处理换行符
			{
				line += '\n';
			}
			else
			{
				_Info[(unsigned char)line[0]]._count = atoi(line.substr(2).c_str());
				line.clear();
			}
			
		}
		//重建哈夫曼树
		CharInfo invalid(0);
		HuffmanTree<CharInfo>ht(_Info, 256, invalid);

		//读取压缩文件，进行还原
		string compressFile = file;
		compressFile += ".compress";
		FILE*fOut = fopen(compressFile.c_str(), "rb");
		assert(fOut);

		string uncompressFile = file;
		uncompressFile += ".uncompress";
		FILE*fIn = fopen(uncompressFile.c_str(), "wb");
		assert(fIn);

		HuffmanTreeNode<CharInfo>*root = ht.GetRoot;
		HuffmanTreeNode<CharInfo>*cur = root;
		char ch = fgetc(fOut);
		int pos = 7;

		while (ChCount != 0)
		{
			if ((unsigned char)ch&(1 << pos))
				cur = cur->_right;
			else
				cur = cur->_left;

			if (cur->_left == NULL&&cur->_right == NULL)
			{
				fputc(cur->_weight._ch, fIn);
				cur = root;
				if (--ChCount == 0)
				{
					return;
				}
			}

			if (pos-- == 0)
			{
				ch = fgetc(fOut);
				pos = 7;
			}
		}
		fclose(fOut);
		fclose(fIn);
		fclose(fConfig);
	}
protected:
	void _CreateHuffCode(HuffmanTreeNode<CharInfo>*root, string&code)
	{
		if (root == NULL)
			return;
		if (root->_left == NULL&&root->_right == NULL)
		{
			_Info[root->_weight._ch]._code = code;
			return;
		}
		_CreateHuffCode(root->_left, code + '0');
		_CreateHuffCode(root->_right, code + '1');
	}
	bool ReadLine(FILE*fOut, string& line)//读取一行
	{
		char ch = fgetc(fOut);
		if (ch == EOF)
			return false;
		while (ch != EOF && ch != '\n')
		{
			line += ch;
			ch = fgetc(fOut);
		}
		return true;
	}
private:
	CharInfo _Info[256];

};
#endif
