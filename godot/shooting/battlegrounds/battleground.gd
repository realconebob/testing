extends Node2D

const ColHandler := preload("res://battlegrounds/collision_handler.gd")
@onready var chandler := ColHandler.new()

@onready var zombies := [$Zombie, $Zombie2]
@onready var player := $Player

func _ready() -> void:
	for gun in (player.get_gunset() as GunManager).get_enabled_guns():
		gun.fired.connect(_handle_bullets)
		
	player.collided.connect(chandler.handle_collision)
	player.died.connect(_kill)
	for zombie in zombies:
		zombie.collided.connect(chandler.handle_collision)
		zombie.died.connect(_kill)
	return

func _physics_process(delta: float) -> void:
	for zombie in zombies:
		if zombie != null && player != null:
			zombie.set_target(player.position)

func _handle_bullets(bullets: Array[BaseEntity]) -> void:
	for bullet in bullets:
		bullet.collided.connect(chandler.handle_collision)
		bullet.died.connect(_kill)
		add_child(bullet)
	
	return
	
func _kill(p: BaseEntity) -> void:
	p.queue_free()
	print("player died")
