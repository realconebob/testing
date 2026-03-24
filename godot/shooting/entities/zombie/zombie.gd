extends BaseEntity

var target: Vector2
var accel_rate: float = 6000

func _physics_process(delta: float) -> void:
	var dir: Vector2 = (target - position).normalized()
	set_acc(dir * accel_rate)
	super(delta)

func set_target(t: Vector2) -> void:
	target = t
