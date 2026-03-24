extends BaseEntity

@onready var gunset: GunManager = GunManager.new()

func _ready() -> void:
	set_max_speed(12000)
	add_child(gunset)

func _physics_process(delta: float) -> void:
	var dir := Input.get_vector(&"move_left", &"move_right", &"move_up", &"move_down")
	set_acc(dir * 8000)
	super(delta)
	
	var mouse_pos: Vector2 = get_global_mouse_position()
	var pointing_vec: Vector2 = (mouse_pos - self.global_position) as Vector2
	var angle: float = pointing_vec.normalized().angle()
	
	gunset.global_rotation = angle
	gunset.position.x = 50 * cos(angle - self.global_rotation)
	gunset.position.y = 50 * sin(angle - self.global_rotation)
