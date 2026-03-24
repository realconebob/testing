extends BaseGun

const bullet_scene := preload("res://guns/bullet.tscn")

@onready var handle: Node2D = $Handle
@onready var barrel: Node2D = $BarrelEnd

func make_bullets(t: Vector2, p: Vector2) -> Array[BaseEntity]:
	var res: Array[BaseEntity] = []
	for i in range(3):
		var bullet: BaseEntity = bullet_scene.instantiate()
		bullet.look_at(target)
		if i > 0:
			bullet.rotate(randf_range(-1, 1) * 1/8 * PI)
		
		bullet.set_max_speed(5000)
		bullet.set_acc((t - p).normalized() * 10000)
		bullet.position = p
		res.append(bullet)
	
	return res

func get_handle() -> Node2D:
	return handle
	
func get_barrel() -> Node2D:
	return barrel
