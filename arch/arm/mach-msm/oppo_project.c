
#include <mach/oppo_project.h>
#include "board-dt.h"
#include <asm/uaccess.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <mach/msm_smem.h>
#include <mach/oppo_reserve3.h>
#include <linux/fs.h>

/////////////////////////////////////////////////////////////
static struct proc_dir_entry *oppoVersion = NULL;
static ProjectInfoCDTType *format = NULL;



unsigned int init_project_version(void)
{
	unsigned int len = (sizeof(ProjectInfoCDTType) + 3)&(~0x3);

	format = (ProjectInfoCDTType *)smem_alloc2(SMEM_PROJECT,len);

	if(format)
		return format->nProject;

	return 0;
}


unsigned int get_project(void)
{
	if(format)
		return format->nProject;
	return 0;
}

unsigned int is_project(OPPO_PROJECT project )
{
	return (get_project() == project?1:0);
}

unsigned char get_PCB_Version(void)
{
	if(format)
		return format->nPCBVersion;
	return 0;
}

unsigned char get_Modem_Version(void)
{
	if(format)
		return format->nModem;
	return 0;
}

unsigned char get_Operator_Version(void)
{
	if(format)
		return format->nOperator;
	return 0;
}


//this module just init for creat files to show which version
static int prjVersion_read_proc(char *page, char **start, off_t off,
			   int count, int *eof, void *data)
{
	unsigned char operator_version;
	int len;
	operator_version = get_Operator_Version();

	BUG_ON(!is_project(OPPO_14013));

	if (operator_version == 3)
		len = sprintf(page,"%d",14016);
	else
		len = sprintf(page,"%d",14013);

	if (len <= off+count)
		*eof = 1;
	*start = page + off;
	len -= off;
	if (len > count)
		len = count;
	if (len < 0)
		len = 0;
	return len;
}

static int pcbVersion_read_proc(char *page, char **start, off_t off,
			   int count, int *eof, void *data)
{
	int len = sprintf(page,"%d",get_PCB_Version());

	if (len <= off+count)
		*eof = 1;
	*start = page + off;
	len -= off;
	if (len > count)
		len = count;
	if (len < 0)
		len = 0;
	return len;
}

static int operatorName_read_proc(char *page, char **start, off_t off,
			   int count, int *eof, void *data)
{
	int len = sprintf(page,"%d",get_Operator_Version());

	if (len <= off+count)
		*eof = 1;
	*start = page + off;
	len -= off;
	if (len > count)
		len = count;
	if (len < 0)
		len = 0;
	return len;
}


static int modemType_read_proc(char *page, char **start, off_t off,
			   int count, int *eof, void *data)
{
	int len = sprintf(page,"%d",get_Modem_Version());

	if (len <= off+count)
		*eof = 1;
	*start = page + off;
	len -= off;
	if (len > count)
		len = count;
	if (len < 0)
		len = 0;
	return len;
}

static int __init oppo_project_init(void)
{
	int ret = 0;
	struct proc_dir_entry *pentry;

	oppoVersion =  proc_mkdir ("oppoVersion", NULL);
	if(!oppoVersion) {
		pr_err("can't create oppoVersion proc\n");
		ret = -ENOENT;
	}

	pentry = create_proc_read_entry ("prjVersion", S_IRUGO, oppoVersion, prjVersion_read_proc,NULL);
	if(!pentry) {
		pr_err("create prjVersion proc failed.\n");
		return -ENOENT;
	}
	pentry = create_proc_read_entry ("pcbVersion", S_IRUGO, oppoVersion, pcbVersion_read_proc,NULL);
	if(!pentry) {
		pr_err("create pcbVersion proc failed.\n");
		return -ENOENT;
	}
	pentry = create_proc_read_entry ("operatorName", S_IRUGO, oppoVersion, operatorName_read_proc,NULL);
	if(!pentry) {
		pr_err("create operatorName proc failed.\n");
		return -ENOENT;
	}
	pentry = create_proc_read_entry ("modemType", S_IRUGO, oppoVersion, modemType_read_proc,NULL);
	if(!pentry) {
		pr_err("create modemType proc failed.\n");
		return -ENOENT;
	}

	return ret;
}

static void __exit oppo_project_init_exit(void)
{

	remove_proc_entry("oppoVersion", NULL);

}

module_init(oppo_project_init);
module_exit(oppo_project_init_exit);


MODULE_DESCRIPTION("OPPO project version");
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Joshua <gyx@oppo.com>");
