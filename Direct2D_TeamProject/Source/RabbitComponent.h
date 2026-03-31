#pragma once
#include "MonoBehaviour.h"

class Inventory;

class RabbitComponent : public MonoBehaviour
{
public:
	RabbitComponent();
	~RabbitComponent();

	Component* Clone() override;

	/// 오브젝트가 씬에 들어가기전에 호출하는 함수
	void Start() override;
	/// 가장 기본적인 업데이트 
	void Update(float _deltaTime) override;
	/// 가장 기본적인 랜더링 이벤트
	void Render(D2DRenderer* _d2DRenderer) override;

	void PostRender(D2DRenderer* _d2DRenderer) override;
	/// 디버그 전용 랜더링 함수 F5를 온오프
	void DebugRender(D2DRenderer* _d2DRenderer) override;

	void OnCollisionEnter(const Collision& _collision) override;
	void OnCollisionStay(const Collision& _collision) override;
	void OnCollisionExit(const Collision& _collision) override;
	void OnTriggerEnter(const Collision& _collision) override;

	GameObject* GetMainPlayer() const { return m_mainPlayer; }
	void SetMainPlayer(GameObject* val) { m_mainPlayer = val; }
	
	bool* GetInputOn() { return &m_isInputOn; }
	bool GetisChangeObject() { return m_isChangeObject; }
	void ReturnToPlayer();
	void ReturnPaticle();
	void ReturnLight();

	//사운드 관련
	void WalkSound();
private:
	// ---------플레이어 관련------------
	GameObject* m_mainPlayer;
	Transform* m_playerTransform;
	Inventory* m_playerInventory;
	RigidBody* m_playerRigidBody;
	BoxCollider* m_boxColiider;

	// ---------------------------------
	float m_deltaTime;
	float m_speed;
	bool m_isInputOn; // 키 입력 활성화 여부
	bool m_isChangeObject;

	float m_walkSoundTime;
	float m_isOneStep;

	Transform* m_transform;
	Animator* m_animator;
	RigidBody* m_rigidBody;
	Inventory* m_inventory;
	Actor* m_actor;

	Vector2 position;

	bool m_isRightDirection;
	bool m_isOnGround;
	bool m_isInverseGravity;
	int m_jumpCount;
	bool m_isJump;
	bool m_isReturnPlayer;
	bool m_isFall;
	//----------토끼 카메라 처리-----------
	const CameraManager* m_myCameraManager;
	Vector2 m_cameraPosition;
	float m_rabbitRangeX;
	float m_rabbitRangeY;
	bool m_isGravitySwtchOn;

 	const wchar_t* rabbitIdle = L"rabbitIdle";
 	const wchar_t* rabbitIdleFlip = L"rabbitIdleFlip";
	const wchar_t* rabbitWalk = L"rabbitWalk";
	const wchar_t* rabbitWalkFlip = L"rabbitWalkFlip";
 	const wchar_t* rabbitJump = L"rabbitJump";
 	const wchar_t* rabbitJumpFlip = L"rabbitJumpFlip";

	//토끼 순간이동
	ParticleSystem* m_paticleSystem;
	float returnPaticlePlayTime;
	PointLightRenderer* m_pointLight;
	float brightness;
	bool m_isReturn;
	Vector2 m_prePosition;
};



