extends BaseEntity

@onready var gunset: GunManager = GunManager.new()

func _ready() -> void:
	set_max_speed(12000)
	add_child(gunset)

func _physics_process(delta: float) -> void:
	var dir := Input.get_vector(&"move_left", &"move_right", &"move_up", &"move_down")
	set_acc(dir * 8000)
	super(delta)
	
	if Input.is_action_just_pressed(&"select_g1"): gunset.set_gun_index(0)
	if Input.is_action_just_pressed(&"select_g2"): gunset.set_gun_index(1)
	if Input.is_action_just_pressed(&"cycle_up"): gunset.set_gun_index(gunset.get_gun_index() + 1)
	if Input.is_action_just_pressed(&"cycle_down"): gunset.set_gun_index(gunset.get_gun_index() - 1)
		# Not ideal but it's fine
	
	var mouse_pos: Vector2 = get_global_mouse_position()
	var pointing_vec: Vector2 = (mouse_pos - self.global_position) as Vector2
	var angle: float = pointing_vec.normalized().angle()
	
	gunset.global_rotation = angle
	gunset.position.x = 50 * cos(angle - self.global_rotation)
	gunset.position.y = 50 * sin(angle - self.global_rotation)

	if Input.is_action_pressed(&"shoot"):
		var gun := gunset.get_current_gun()
		gun.set_target(get_global_mouse_position())
		gunset.fire()
		print("player: tried to fire gun")
		
func get_gunset() -> GunManager:
	return gunset
