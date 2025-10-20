we're making a struct `t_scene`:
```
typedef struct s_scene{
	t_camera *camera;
	t_light *ambient;
	t_light **lights;
	size_t num_lights;
	t_object **objects;
	size_t num_objects;
} t_scene;
```

```
typedef struct s_camera{
	double position[3];
	double direction[3];
} t_camera;
```

```
typedef struct s_light{
	double position[3];
	double direction[3];
	bool is_ambient;
	//int32_t color;
}
```

```
typedef struct s_object{
	e_type type;
	double position[3];
	double var1;
	double var2;
}
```

```
typedef enum e_type{
	SPHERE,
	PLANE,
	CYLINDER
}
```
