#include <iostream>
#include <string.h>
#include <stdio.h>

#if 1
	#define TRACE(x) { std::cout << "[" << x << "] " << __FUNCTION__ << std::endl; }
#else
	#define TRACE(x) { }
#endif

class String
{
public:
	String():m_str(NULL), m_size(0) 
	{
		TRACE("default");
	}
	
	String(const char *str)
	{
		TRACE("normal");
		if(str)
		{
	    	m_size = strlen(str);
	    	m_str = new (std::nothrow) char[m_size+1];
			// 内存分配失败的处理。new失败默认是抛异常，通过声明std::nothrow不抛异常，返回NULL。
			if(m_str == NULL)
			{
				m_str = NULL;
				m_size = 0;
				return;
			}
	    	memcpy(m_str, str, m_size); 
			// 设置结尾符
	    	m_str[m_size] = '\0';       
		}else{
			// 传入NULL的处理
			m_str = NULL;
			m_size = 0;
		}
	}
	
	String(const String &str)
	{
		// if empty

		TRACE("copy");
	    m_size = str.m_size;
	    m_str = new (std::nothrow) char[m_size+1];
		if(m_str == NULL)
		{
			m_str = NULL;
			m_size = 0;
			return;
		}
	    memcpy(m_str, str.m_str, m_size); 
	    m_str[m_size] = '\0';       
	}
	
	~String()
	{
		TRACE("deconstructor");
	    if(m_str) delete[] m_str;
	    m_size = 0;
	}
	
	// 必须返回引用，否则无法实现a = b = c;的赋值方式。
	String &operator = (const String &str)
	{
		TRACE("assign");

		// 判断是否为同一实例，否则会有导致内存泄漏和拷贝不正确的问题
		if(this == &str) return *this;

		// 释放已有的内存
		if(m_str)
		{
			delete[] m_str;
			m_size = 0;
		}

		m_str = new (std::nothrow) char[str.m_size+1];
		if(m_str == NULL)
		{
			m_str = NULL;
			m_size = 0;
			*this;
		}
		memcpy(m_str, str.m_str, str.m_size); 
		m_str[str.m_size] = '\0';
		m_size = str.m_size;
		return *this;
	}
	
	/*
		friend String operator + (const char *s1, const char *s2);
		friend String operator + (const String &s1, const char *s2);
		friend String operator + (const char *s2, const String &s1);
	*/

	// 必须声明为friend的外部函数才可以a = b + c;
	friend String operator + (const String &s1, const String &s2)
	{
		TRACE("-");
		String i_str;
		i_str.m_size = s1.m_size + s2.m_size;
		if(i_str.m_size == 0)
		{
			i_str.m_str = NULL;
			i_str.m_size = 0;
			return i_str;
		}

		i_str.m_str = new (std::nothrow) char[i_str.m_size + 1];
		if(i_str.m_str == NULL)
		{
			i_str.m_str = NULL;
			i_str.m_size = 0;
			return i_str;
		}

		if(!s1.empty())
		{
			memcpy(i_str.m_str, s1.m_str, s1.m_size); 
		}

		if(!s2.empty())
		{
			memcpy(i_str.m_str+s1.m_size, s2.m_str, s2.m_size); 
		}

		i_str.m_str[i_str.m_size] = '\0'; 

		return i_str;
	}

	String &operator += (const String &s1)
	{
		TRACE("-");
		return this + s1;
		/*
		String i_str;
		i_str.m_size = this.m_size + s1.m_size;
		i_str.m_str = new (std::nothrow) char[i_str.m_size + 1];
		if(i_str.m_str == NULL)
		{
			i_str.m_str = NULL;
			i_str.m_size = 0;
			return i_str;
		}
		memcpy(i_str.m_str, this.m_str, s1.m_size); 
		memcpy(i_str.m_str+s1.m_size, s1.m_str, s1.m_size); 
		i_str.m_str[i_str.m_size] = '\0'; 
		return i_str;
		*/
	}

	const char *c_str()
	{
		if(m_str)
		{ 
			return m_str;
		}else{
			return NULL;
		}
	}
	
	int size() const
	{
		return m_size;
	}

	bool empty() const
	{
	    if(m_size == 0 || m_str == NULL)
	 	{
	 		return true;
	 	}
	 	return false;
	}

	friend std::ostream &operator << (std::ostream& out, const String &str)
	{
		if(!str.empty())
		{
			out << str.m_str;
		}else{
			out << "(null)" ;
		}
		return out;
	}

private:
	char *m_str;
	int m_size;
};

int main()
{
	printf("------1------\n");
	String str("Hello world");
	std::cout << str << std::endl;

	printf("------2------\n");
	String str2(", this is C++");
	String str3 = str + str2;
	std::cout << str3 << std::endl;

	printf("------2.1------\n");
	String str31;
	str31 = str + str2;
	std::cout << str31 << std::endl;
	
	printf("------3------\n");
	char *p = NULL;
	String str4(p);
	std::cout << str4 << std::endl;

	printf("------4------\n");
	String s1 = str;
	std::cout << s1 << std::endl;

	printf("------5------\n");
	String s2;
	s2 = str;
	std::cout << s2 << std::endl;

	printf("------6------\n");
	String s3, s4;
	s3 = s4 = str3;
	std::cout << s3 << std::endl;
	std::cout << s4 << std::endl;
}
