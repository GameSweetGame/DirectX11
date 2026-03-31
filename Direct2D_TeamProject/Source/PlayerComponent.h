#pragma once
#include "MonoBehaviour.h"

class Inventory;
class FreezeEffect;
class PlayerLife;
class Actor;

class PlayerComponent : public MonoBehaviour
{
public:
	PlayerComponent();
	~PlayerComponent();

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

	bool GetChangeObject() const { return m_changeObject; }
	void SetChangeObject(bool val) { m_changeObject = val; }

	bool GetIsInverseGravity() const { return m_isInverseGravity; }
	void SetIsInverseGravity(bool val) { m_isInverseGravity = val; }
	
	void InverseGravityAnimation(float dt);
	
	bool IsPlayerActive() { return !m_changeObject; }
	GameObject* GetRabbit() const { return m_rabbit; }
	void SetRabbit(GameObject* _rabbit) { m_rabbit = _rabbit; }
	bool* GetInputOn() { return &m_isInputOn; }
	float GetSpeed() const { return m_speed; }
	void SetSpeed(float _speed) { m_speed = _speed; }
	float GetJumpPower() const { return m_jumpPower; }
	void SetJumpPower(float val) { m_jumpPower = val; }
	bool IsFreeze() const { return m_isFreeze; }
	void SetFreeze(bool val) { m_isFreeze = val; }


	/// 애니메이션 관련
	void KeepRunnig();			//좌우 이동키 조작 시 애니메이션 자연스럽게
	void WalkAnimationCheck();	// 25프레임 시간을 측정
	void JumpAnimation();

	bool GetGravitySwtchOn() const { return m_isGravitySwtchOn; }
	void SetGravitySwtchOn(bool val) { m_isGravitySwtchOn = val; }
	bool GetRightDirection() const { return m_isRightDirection; }
	void SetRightDirection(bool val) { m_isRightDirection = val; }
	PlayerLife* GetPlayerLife() const { return m_playerLife; }
	void SetPlayerLife(PlayerLife* val) { m_playerLife = val; }

	/// 사운드 관련
	void WalkSound();
	bool GetStop() const { return m_rStop; }
	void SetRStop(bool val) { m_rStop = val; }
private:
	
	GameObject* m_rabbit;
	float m_speed;
	float m_jumpPower;
	bool m_isInputOn; // 키입력을 받는 상태 
	bool m_isFreeze; // 언 상태인지
	float m_deltaTime;
	float m_changeAnimationTime;
	float m_keepRunningTime;
	float m_walkSoundTime;
	bool m_isOneStep;
	float m_jumpTime;
	bool m_isJumpControl;
	bool m_isCanJump;
	bool m_rStop;

	// 받아온 컴포넌트
	Transform* m_transform;
	Animator* m_animator;
	RigidBody* m_rigidBody;
	PlayerLife* m_playerLife;

	BoxCollider* m_boxCollider;
	Inventory* m_inventory;
	Inventory* m_rabbitInventory;
	FreezeEffect* m_freezeEffect;
	Actor* m_actor;
	int m_walkStyle;
	//임시
	//BoxRenderer* m_boxRenderer;
	
	Vector2 m_position;
	Vector2 m_prevPosition;

	// 받아온 매니저
	const InputManager* m_myInputManager;
	const CollisionManager* m_myCollisionManager;
	const CameraManager* m_myCameraManager;
	
	bool m_isRightDirection;	// 오른쪽 방향 확인, 애니메이션을 결정할 때 사용
	bool m_isOnGround;		 
	bool m_isInverseGravity;
	bool m_isGravitySwtchOn;
	int m_jumpCount;
	bool m_changeObject;		// 토끼로 움직일 때 사용
	bool m_isJump;
	bool m_isKeyDown;
	bool m_isChangeAnimationOn;
	bool m_isRun;
	UINT m_isOnIce;  //옆으로 닿았다가 떨어졌을 때 미끄러지는게 풀려서 정수형으로 했습니다
	bool m_isFirstKey;
	bool m_isPlayInverseAnimaition;
	float m_playInverseAnimaitionTime;
	bool m_isPrevRightDirection;
	float m_lateJumpTime;
	// 애니메이션 이름
	const wchar_t* playerIdle = L"playerIdle";
	const wchar_t* playerIdleFlip = L"playerIdleFlip";
	const wchar_t* playerWalk = L"playerWalk";
	const wchar_t* playerWalkFlip = L"playerWalkFlip";
	const wchar_t* playerJump = L"playerJump";
	const wchar_t* playerJumpFlip = L"playerJumpFlip";	
	
	const wchar_t* playerJumping = L"playerJumping";
	const wchar_t* playerJumpingFlip = L"playerJumpingFlip";

// 	const wchar_t* playerIdleToWalk = L"playerIdleToWalk";
// 	const wchar_t* playerIdleToWalkFlip = L"playerIdleToWalkFlip";	
	const wchar_t* playerInverseGravity = L"playerInverseGravity";
	const wchar_t* playerInverseGravityFlip = L"playerInverseGravityXFlip";
	
	const wchar_t* playerWalkToIdle = L"playerWalkToIdle";
	const wchar_t* playerWalkToIdleFlip = L"playerWalkToIdleFlip";

	const wchar_t* playerFreeHand = L"playerFreeHand";
	const wchar_t* playerFreeHandFlip = L"playerFreeHandFlip";

	const wchar_t* playerFreezeIdle = L"playerFreezeIdle";
	const wchar_t* playerFreezeIdleFlip = L"playerFreezeIdleFlip";
	const wchar_t* playerFreezeWalk = L"playerFreezeWalk";
	const wchar_t* playerFreezeWalkFlip = L"playerFreezeWalkFlip";
	const wchar_t* playerFreezeJump = L"playerFreezeJump";
	const wchar_t* playerFreezeJumpFlip = L"playerFreezeJumpFlip";
	// 중력 애니메이션 관련
	vector<Collider*> m_colliderVector;
};

