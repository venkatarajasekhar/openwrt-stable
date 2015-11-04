/*
 *  NXP 74HC153 - Dual 4-input multiplexer GPIO driver
 *
 *  Copyright (C) 2010 Gabor Juhos <juhosg@openwrt.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 */

#include <linux/version.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/nxp_74hc153.h>

#define NXP_74HC153_NUM_GPIOS	8
#define NXP_74HC153_S0_MASK	0x1
#define NXP_74HC153_S1_MASK	0x2
#define NXP_74HC153_BANK_MASK	0x4

typedef struct nxp_74hc153_chip_s {
	struct device		*parent;
	struct gpio_chip	gpio_chip;
	struct mutex		lock;
}nxp_74hc153_chip_t;

static nxp_74hc153_chip_t* gpio_to_nxp(struct gpio_chip *gc)
{
	return container_of(gc, nxp_74hc153_chip_t, gpio_chip);
}

static int nxp_74hc153_direction_input(struct gpio_chip *gc, unsigned offset)
{
	return 0;
}

static int nxp_74hc153_direction_output(struct gpio_chip *gc,
					unsigned offset, int val)
{
	return -EINVAL;
}

static int nxp_74hc153_get_value(struct gpio_chip *gc, unsigned char offset)
{
	nxp_74hc153_chip_t *ptrto_74hc153_chip,*ptrto_74hc153;
	struct nxp_74hc153_platform_data *ptrto_74hc153_platform_data,*ptr__74hc153_platform_data;
	unsigned char s0;
	unsigned char s1;
	unsigned char pin;
	int ret;
        if(ptrto_74hc153_chip){
	ptrto_74hc153 = gpio_to_nxp(gc);
	
	if(ptrto_74hc153_platform_data){
	ptr__74hc153_platform_data = ptrto_74hc153->parent->platform_data;
	s0 = (offset & NXP_74HC153_S0_MASK);
	s1 = (offset & NXP_74HC153_S1_MASK);
	pin =(offset & NXP_74HC153_BANK_MASK) ? ptr__74hc153_platform_data->gpio_pin_2y
					       : ptr__74hc153_platform_data->gpio_pin_1y;

	mutex_lock(&nxp->lock);
	gpio_set_value(ptr__74hc153_platform_data->gpio_pin_s0, s0);
	gpio_set_value(ptr__74hc153_platform_data->gpio_pin_s1, s1);
	}else{
	dev_err(&pdev->dev, "Error :Memory allocation \n");
		return -ENOMEM;		
	}
        } //if ptrto_74hc153_chip
        else{
        dev_err(&pdev->dev, "Error :Memory allocation \n");
		return -ENOMEM;	
        }//else ptrto_74hc153_chip
	ret = gpio_get_value(pin);
	mutex_unlock(&nxp->lock);

	return ret;
}

static void nxp_74hc153_set_value(struct gpio_chip *gc,
				  unsigned offset, int val)
{
	/* not supported */
}

static int nxp_74hc153_probe(struct platform_device *pdev)
{
	struct nxp_74hc153_platform_data *pdata;
	//struct nxp_74hc153_chip *nxp;
	nxp_74hc153_chip_t* ptrto_4hc153_chip_data;
	struct gpio_chip *gc;
	int err;

	pdata = pdev->dev.platform_data;
	if (pdata == NULL) {
		dev_dbg(&pdev->dev, "no platform data specified\n");
		return -EINVAL;
	}

	ptrto_4hc153_chip_data = kzalloc(sizeof(nxp_74hc153_chip_t), GFP_KERNEL);
	if (ptrto_4hc153_chip_data == NULL) {
		dev_err(&pdev->dev, "no memory for private data\n");
		return -ENOMEM;
	}

	err = gpio_request(pdata->gpio_pin_s0, dev_name(&pdev->dev));
	if (err) {
		dev_err(&pdev->dev, "unable to claim gpio %u, err=%d\n",
			pdata->gpio_pin_s0, err);
		goto err_free_nxp;
	}

	err = gpio_request(pdata->gpio_pin_s1, dev_name(&pdev->dev));
	if (err) {
		dev_err(&pdev->dev, "unable to claim gpio %u, err=%d\n",
			pdata->gpio_pin_s1, err);
		goto err_free_s0;
	}

	err = gpio_request(pdata->gpio_pin_1y, dev_name(&pdev->dev));
	if (err) {
		dev_err(&pdev->dev, "unable to claim gpio %u, err=%d\n",
			pdata->gpio_pin_1y, err);
		goto err_free_s1;
	}

	err = gpio_request(pdata->gpio_pin_2y, dev_name(&pdev->dev));
	if (err) {
		dev_err(&pdev->dev, "unable to claim gpio %u, err=%d\n",
			pdata->gpio_pin_2y, err);
		goto err_free_1y;
	}

	err = gpio_direction_output(pdata->gpio_pin_s0, 0);
	if (err) {
		dev_err(&pdev->dev,
			"unable to set direction of gpio %u, err=%d\n",
			pdata->gpio_pin_s0, err);
		goto err_free_2y;
	}

	err = gpio_direction_output(pdata->gpio_pin_s1, 0);
	if (err) {
		dev_err(&pdev->dev,
			"unable to set direction of gpio %u, err=%d\n",
			pdata->gpio_pin_s1, err);
		goto err_free_2y;
	}

	err = gpio_direction_input(pdata->gpio_pin_1y);
	if (err) {
		dev_err(&pdev->dev,
			"unable to set direction of gpio %u, err=%d\n",
			pdata->gpio_pin_1y, err);
		goto err_free_2y;
	}

	err = gpio_direction_input(pdata->gpio_pin_2y);
	if (err) {
		dev_err(&pdev->dev,
			"unable to set direction of gpio %u, err=%d\n",
			pdata->gpio_pin_2y, err);
		goto err_free_2y;
	}

	//nxp->parent = &pdev->dev;
	//mutex_init(&nxp->lock);

        //gc = &nxp->gpio_chip;
        ptrto_4hc153_chip_data->parent = &pdev->dev;
        mutex_init(&ptrto_4hc153_chip_data->lock);
        gc = &ptrto_4hc153_chip_data->gpio_chip;
        
	gc->direction_input  = nxp_74hc153_direction_input;
	gc->direction_output = nxp_74hc153_direction_output;
	gc->get = nxp_74hc153_get_value;
	gc->set = nxp_74hc153_set_value;
	gc->can_sleep = 1;

	gc->base = pdata->gpio_base;
	gc->ngpio = NXP_74HC153_NUM_GPIOS;
	gc->label = dev_name(ptrto_4hc153_chip_data->parent);
	gc->dev = ptrto_4hc153_chip_data->parent;
	gc->owner = THIS_MODULE;

	err = gpiochip_add(&ptrto_4hc153_chip_data->gpio_chip);
	if (err) {
		dev_err(&pdev->dev, "unable to add gpio chip, err=%d\n", err);
		goto err_free_2y;
	}

	platform_set_drvdata(pdev, ptrto_4hc153_chip_data);
	return 0;

err_free_2y:
	gpio_free(pdata->gpio_pin_2y);
err_free_1y:
	gpio_free(pdata->gpio_pin_1y);
err_free_s1:
	gpio_free(pdata->gpio_pin_s1);
err_free_s0:
	gpio_free(pdata->gpio_pin_s0);
err_free_nxp:
	kfree(ptrto_4hc153_chip_data);
	return err;
}

static int nxp_74hc153_remove(struct platform_device *pdev)
{
	//struct nxp_74hc153_chip *nxp = platform_get_drvdata(pdev);
	nxp_74hc153_chip_t* ptrto_4hc153_chip_data,*ptr_4hc153_chip_data;
	if(pdev){
	ptrto_4hc153_chip_data = platform_get_drvdata(pdev);
	}else{
		
	}
	struct nxp_74hc153_platform_data *pdata;
	if(pdata){
	pdata = pdev->dev.platform_data;
	}else{
		
	}
	if (ptrto_4hc153_chip_data) {
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,15,0)
		int err;

		err = gpiochip_remove(&ptr_4hc153_chip_data->gpio_chip);
		if (err) {
			dev_err(&pdev->dev,
				"unable to remove gpio chip, err=%d\n",
				err);
			return err;
		}
#else
		gpiochip_remove(&ptr_4hc153_chip_data->gpio_chip);
#endif
		gpio_free(pdata->gpio_pin_2y);
		gpio_free(pdata->gpio_pin_1y);
		gpio_free(pdata->gpio_pin_s1);
		gpio_free(pdata->gpio_pin_s0);

		kfree(ptr_4hc153_chip_data);
		platform_set_drvdata(pdev, NULL);
	}

	return 0;
}

static struct platform_driver nxp_74hc153_driver = {
	.probe		= nxp_74hc153_probe,
	.remove		= nxp_74hc153_remove,
	.driver = {
		.name	= NXP_74HC153_DRIVER_NAME,
		.owner	= THIS_MODULE,
	},
};

static int __init nxp_74hc153_init(void)
{
	return platform_driver_register(&nxp_74hc153_driver);
}
subsys_initcall(nxp_74hc153_init);

static void __exit nxp_74hc153_exit(void)
{
	platform_driver_unregister(&nxp_74hc153_driver);
}
module_exit(nxp_74hc153_exit);

MODULE_AUTHOR("Gabor Juhos <juhosg@openwrt.org>");
MODULE_DESCRIPTION("GPIO expander driver for NXP 74HC153");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:" NXP_74HC153_DRIVER_NAME);
