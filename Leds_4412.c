#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/delay.h>
 
#include <linux/gpio.h>
#include <mach/gpio.h>
#include <plat/gpio-cfg.h>
#include <linux/gpio.h>
//modfile
//modfile
//ssss
struct led_desc{
	int gpio;
	char *name;
};

static led_desc led_gpios[] = {
	{EXYNOS4212_GPM4(0),"LED1"}
	{EXYNOS4212_GPM4(1),"LED2"}
	{EXYNOS4212_GPM4(2),"LED3"}
	{EXYNOS4212_GPM4(3),"LED4"}
};
//s3c_gpio_cfgpin(led_gpios[i], S3C_GPIO_OUTPUT);
//gpio_set_value(led_gpios[arg], !cmd);




struct led_classdev_4412{
	struct led_classdev led_cdev;
	int gpio;
};
static struct led_classdev_4412 *led_4412_devs;

static void brightness_set_4412(struct led_classdev *led_cdev,enum led_brightness brightness)
{
	led_classdev_4412 *dev = (led_classdev_4412 *)led_cdev;
	led_cdev->brightness = brightness;
	if(brightness != LED_OFF)
	{
		gpio_set_value(dev->gpio, 0);
	}
	else
	{
		gpio_set_value(dev->gpio, 0);
	}

}
int leds_init(void)
{
	int i,ret;
	led_4412_devs = kzalloc(sizeof(struct led_classdev_4412)*(sizeof(led_gpios)/sizeof(led_gpios[0])), 
						GFP_KERNEL);
	if (!led_4412_devs)
		return -ENOMEM;
	for(i = 0; i < (sizeof(led_gpios)/sizeof(led_gpios[0]));i++)
	{
		s3c_gpio_cfgpin(led_gpios[i].gpio, S3C_GPIO_OUTPUT);//配置为输出引脚
		gpio_set_value(led_gpios[i].gpio, 1);
		led_4412_devs[i].led_cdev.max_brightness = LED_FULL;
		led_4412_devs[i].led_cdev.brightness_set = brightness_set_4412;
		led_4412_devs[i].led_cdev.flags = LED_CORE_SUSPENDRESUME;
		led_4412_devs[i].led_cdev.brightness = LED_OFF;
		led_4412_devs[i].led_cdev.name = led_gpios[i].name;
		led_4412_devs[i].gpio = led_gpios[i].gpio;
		ret = led_classdev_register(NULL,&led_4412_devs[i].led_cdev);
		if(ret < 0)
		{
			i--;
			while(i >= 0){
				led_classdev_unregister(&led_4412_devs[i].led_cdev);
				i--;
			}
			kfree(led_4412_devs);
			return ENODEV;
		}
		
	}
	return 0;
}

void leds_exit(void)
{
	int i;

	for(i = 0; i < (sizeof(led_gpios)/sizeof(led_gpios[0]));i++){
		led_classdev_unregister(&led_4412_devs[i]);		
	}
	kfree(led_4412_devs.led_cdev);
}

module_init(leds_init);
module_exit(leds_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("lijun Inc.");
