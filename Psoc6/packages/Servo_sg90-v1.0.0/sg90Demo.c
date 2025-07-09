#include "sg90.h"

// 测试pwm0舵机
static int set_angle0(int argc,char **argv)
{
    
    sg90_cfg_t cfg =&my_sg90_config0;
    
    
    if (cfg->name==RT_NULL)
    {
        /* code */
        sg90_init(cfg,0);
    }
        

    if(argc==2)
    {
        sg90_set_angle(cfg,atoi(argv[1]));
        sg90_get(cfg);
    }
    
    return 0;
}

MSH_CMD_EXPORT(set_angle0,set angle);

// 测试pwm1舵机
static int set_angle1(int argc,char **argv)
{
    
    sg90_cfg_t cfg =&my_sg90_config1;
    
    
    if (cfg->name==RT_NULL)
    {
        /* code */
        sg90_init(cfg,1);
    }
        

    if(argc==2)
    {
        sg90_set_angle(cfg,atoi(argv[1]));
        sg90_get(cfg);
    }
    
    return 0;
}

MSH_CMD_EXPORT(set_angle1,set angle);
