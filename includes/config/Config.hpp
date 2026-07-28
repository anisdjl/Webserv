#ifndef CONFIG
# define CONFIG

class Config
{
	public:
		Config(void);
		Config(const Config &src);
		Config	&operator=(const Config &src);
		~Config();
};

#endif