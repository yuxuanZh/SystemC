#include "device-list.h"
#include "alloc.h"

#include "debayer-sync.h"

static void debayer_release(struct object *obj)
{
	struct debayer_sync *debayer = obj_to_debayer(obj);

	free(debayer);
}

static int debayer_create(const struct device_desc *desc, const char *name)
{
	struct debayer_sync *debayer;

	debayer = cargo_zalloc(sizeof(*debayer));
	if (debayer == NULL)
		return -1;

	debayer->dev.obj.release = debayer_release;
	debayer->dev.obj.name = name;
	debayer->dev.id = desc->id;
	debayer->dev.length = DEBAYER_NR_REGS * sizeof(u32);

	if (device_sync_register(&debayer->dev, debayer_main)) {
		free(debayer);
		return -1;
	}

	return 0;
}

const struct device_init_operations debayer_sync_init_ops = {
	.create		= debayer_create,
};
