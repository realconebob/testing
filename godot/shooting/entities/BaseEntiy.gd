class_name BaseEntity
extends CharacterBody2D

signal collided(collider: KinematicCollision2D, collidee: BaseEntity)
signal damaged(this: BaseEntity)
signal died(this: BaseEntity)

var max_speed: float = 1200
var damping: float = 1.25

var health: float = 100
var alive: bool = true

var accel: Vector2 = Vector2.ZERO

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _physics_process(delta: float) -> void:
	velocity += Vector2(
		velocity.x * ((1/damping) - 1),
		velocity.y * ((1/damping) - 1),
	) + (delta * accel)
	
	velocity = velocity.clamp(
		Vector2.ONE * -max_speed, 
		Vector2.ONE * max_speed
	)
	
	var collider := move_and_collide(velocity * delta)
	if collider:
		collided.emit(collider, self)
	
	pass

func set_acc(acc: Vector2) -> void:
	accel = acc
	
func get_acc() -> Vector2:
	return accel
	
func set_max_speed(speed: float) -> void:
	max_speed = abs(speed)
	
func get_max_speed() -> float:
	return max_speed
	
func set_health(new_hp: float) -> void:
	if !alive: return
	
	var last_hp: float = health
	health = max(0, min(new_hp, 100))
	
	if health < last_hp:
		damaged.emit(self)
	if health <= 0:
		alive = false
		died.emit(self)
		
func get_health() -> float:
	return health
