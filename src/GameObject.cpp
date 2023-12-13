#include "GameObject.h"

GameObject::GameObject() :
	m_rigidBody(new RigidBody()),
	m_collider(new Box(m_rigidBody, &m_rigidBody->getPosition(), new Vector3D(1, 0, 0), new Vector3D(0, 1, 0), new Vector3D(0, 0, 1))),
	m_encompassingSphere(new Sphere(m_rigidBody, &m_rigidBody->getPosition(), 2))
{}

GameObject::GameObject(RigidBody* rigidBody, Box* boxCollider, Sphere* encompassingSphere) :
	m_rigidBody(rigidBody),
	m_collider(boxCollider),
	m_encompassingSphere(encompassingSphere)
{}

GameObject::~GameObject()
{
	delete m_rigidBody;
	delete m_collider;
	delete m_encompassingSphere;
}

void GameObject::update()
{
	m_rigidBody->Update();
	m_collider->update();
	m_encompassingSphere->update();
}

void GameObject::draw(ofColor color)
{
	ofSetColor(color);
	for (of3dPrimitive* primitive : m_rigidBody->getPrimitives())
	{
		primitive->draw();
	}

	ofSetColor(255, 0, 0);
	m_collider->draw();

	ofSetColor(0, 255, 0);
	m_encompassingSphere->draw();
}

RigidBody* GameObject::getRigidBody() { return m_rigidBody; }
Collider* GameObject::getCollider() { return m_collider; }
Sphere* GameObject::getSphere() { return m_encompassingSphere; }