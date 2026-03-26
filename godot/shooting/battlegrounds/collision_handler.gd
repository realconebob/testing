extends Node

func handle_collision(collider: KinematicCollision2D, collidee: BaseEntity) -> void:
	# Need to handle collisions between:
		# Walls: Players, Enemies, and Bullets
		# Players: Enemies
		# Enemies: Players, Enemies, and Bullets
		# Bullets: Enemies
		
	if collider.get_collider().has_method("get_type"):
		if (collider.get_collider().call("get_type") as String) == "Wall":
			match collidee.get_type():
				"Bullet":
					collidee.queue_free()
					return
				
				"Player", "Zombie":
					# Translate movement to normal without losing speed
					#collidee.set_acc(collidee.get_acc().slide(collider.get_normal()).normalized() * collidee.get_acc().length())
					#collidee.velocity = collidee.velocity.slide(collider.get_normal()).normalized() * collidee.velocity.length()
					
					# Bounce
					collidee.set_acc(collidee.get_acc().bounce(collider.get_normal()))
					collidee.velocity = collidee.velocity.bounce(collider.get_normal())
					return
			
			return
	
	if collidee.get_type() == "Player":
		if !collider.get_collider().has_method("get_type"): return
		if collider.get_collider().call("get_type") == "Zombie":
				collidee.set_health(collidee.get_health() - 1)
				collidee.set_acc(collidee.get_acc().bounce(collider.get_normal()).normalized() * collidee.get_acc().length() * 100)
				collidee.velocity = collidee.velocity.bounce(collider.get_normal())
				return
			
		return
		
	if collidee.get_type() == "Zombie":
		if !collider.get_collider().has_method("get_type"): return
		match collider.get_collider().call("get_type"):
			"Player":
				var player: BaseEntity = collider.get_collider()
				player.set_health(player.get_health() - 1)
				
				collidee.set_acc(collidee.get_acc().bounce(collider.get_normal()).normalized() * collidee.get_acc().length() * 100)
				collidee.velocity = collidee.velocity.bounce(collider.get_normal()) * 10
				return
				
			"Zombie":
				collidee.set_acc(collidee.get_acc().bounce(collider.get_normal()))
				collidee.velocity = collidee.velocity.bounce(collider.get_normal())
				return
				
			"Bullet":
				var bullet: BaseEntity = collider.get_collider()
				var zhealth := collidee.get_health()
				var bhealth := bullet.get_health()
				
				collidee.set_health(zhealth - bhealth)
				bullet.set_health(bhealth - zhealth)
				return
			
		return
	
	return
