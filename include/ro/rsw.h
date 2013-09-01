#ifndef RSW_H
#define RSW_H

/*
Rsw Header

Main	Size : 246 bytes
*	Field	Size	Type	Description

byte[4]  magic             // "GRSW" magic bytes
byte	 version_major
byte     version_minor
byte[40] gnd_file
byte[40] gat_file

float32  water_depth
uint32   water_type
float32  water_amplitude
float32  water_phase
float32  water_curve
uint32   water_cycles

float32[3]  light_ambient // r g b
float32[3]  light_diffuse // r g b
float32[3]  light_shadow  // r g b
float32  light_alpha

byte[4]  unknown
uint32   objects_count
objects {
	uint32	object_type
	byte[]  ---> varies based on object_type.
}

--- object_type 1 (model_instance)
byte[40]   name (unique)
uint32     animation_type
float32    animation_speed
uint32     block_type
byte[80]   model_filename
byte[80]   model_type  // Objectxx | Cylinderxx | Boxx | GeoSphereXX | MeshXX | SphereXX
float32[3] pos         // x y z
float32[3] rot         // x y z
float32[3] scale       // x y z

--- object_type 2 (light)
byte[80]   name
float[3]   pos     // x y z
float32[3] color   // r g b
float32    range

--- object_type 3 (sound)
byte[80]   name
byte[80]   wav_name
float32[3] pos       // x y z
float32    vol
uint32     width
uint32     height
float32    range
float32    cycle

--- object_type 4 (effect)
byte[80]   name;
float[3]   pos; 	// x y z
uint32     type;
float32    speed;
float32[4] param;

*/

#endif