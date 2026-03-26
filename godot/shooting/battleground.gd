extends Node2D

func _ready() -> void:
	for gun in ($Player.get_gunset() as GunManager).get_enabled_guns():
		gun.fired.connect(_handle_bullets)
	return

func _physics_process(delta: float) -> void:
	$Zombie.set_target($Player.position)

func _handle_bullets(bullets: Array[BaseEntity]) -> void:
	for bullet in bullets:
		add_child(bullet)
	
	return
