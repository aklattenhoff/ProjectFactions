#ifndef _ORTHO_DEPTH_H
#define _ORTHO_DEPTH_H

#ifdef __cplusplus
extern "C" {
#endif

enum OrthoLayer
{
	ORTHO_CURSOR,
	ORTHO_TEXT,
	ORTHO_OBJECT_ICON,
	ORTHO_OBJECT_PANEL,
	ORTHO_ACTION_PANEL,
	ORTHO_RESOURCE_PANEL,
	ORTHO_MINIMAP_PANEL,
	ORTHO_HEALTH_INDICATOR,
	ORTHO_SELECTION_BOX,
	ORTHO_RECTANGLE,
};

extern inline float OrthoDepth(enum OrthoLayer orthoLayer);

#ifdef __cplusplus
}
#endif

#endif /* _ORTHO_DEPTH_H */
