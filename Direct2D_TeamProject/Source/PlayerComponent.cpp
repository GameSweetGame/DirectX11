#include "UserScriptsPCH.h"
#include "PlayerComponent.h"
#include "Inventory.h"
#include "SlideTile.h"
#include "Icicle.h"
#include "FreezeEffect.h"
#include "PassPlatform.h"
#include "Actor.h"
#include "PlayerLife.h"
#include <random>

// 캐릭터 이동, 역중력, 애니메이션 상태를 관리하는 컴포넌트
PlayerComponent::PlayerComponent()
	:MonoBehaviour(typeid(this).name()), m_speed(0.f), m_transform(nullptr), m_animator(nullptr), m_isRightDirection(true), m_isOnGround(false), m_rigidBody(nullptr)
	, m_jumpCount(0), m_isInverseGravity(false), m_changeObject(false), m_isGravitySwtchOn(false), m_rabbit(nullptr), m_inventory(nullptr), m_rabbitInventory(nullptr)
	, m_myInputManager(nullptr), m_myCollisionManager(nullptr), m_isJump(false), m_boxCollider(nullptr), m_isInputOn(false), m_myCameraManager(nullptr), m_isRun(false)
	, m_changeAnimationTime(0.f), m_deltaTime(0.f), m_isChangeAnimationOn(false), m_isKeyDown(false), m_keepRunningTime(0.f), m_isOnIce(0), m_jumpPower(0.f), m_isFreeze(false)
	, m_freezeEffect(nullptr), m_isFirstKey(false), m_playerLife(nullptr), m_isOneStep(false), m_walkSoundTime(0.f), m_jumpTime(0.f), m_lateJumpTime(0.f), m_playInverseAnimaitionTime(0.f)
	, m_isPlayInverseAnimaition(false), m_isPrevRightDirection(true), m_actor(nullptr), m_isJumpControl(false), m_walkStyle(0), m_rStop(false)
{
}

PlayerComponent::~PlayerComponent()
{
}

Component* PlayerComponent::Clone()
{
	PlayerComponent* clone = new PlayerComponent();
	return clone;
}

void PlayerComponent::Start()
{
	// 필요한 컴포넌트 캐싱
	m_transform = GetComponent<Transform>();
	m_animator = GetComponent<Animator>();
	m_rigidBody = GetComponent<RigidBody>();
	m_boxCollider = GetComponent<BoxCollider>();
	m_freezeEffect = GetComponent<FreezeEffect>();
	m_actor = GetComponent<Actor>();

	// 인벤토리 초기화 (플레이어 + 토끼 공유)
	m_inventory = GetComponent<Inventory>();
	m_rabbitInventory = m_rabbit->GetComponent<Inventory>();

	m_speed = 300.f;
	m_jumpPower = 600.f;
	m_jumpCount = 0;
	m_isInputOn = true;

	// 매니저 캐싱
	m_myInputManager = GetInputManager();
	m_myCollisionManager = GetCollisionManager();
	m_myCameraManager = GetCameraManager();

	m_changeAnimationTime = 0.416f;	// 25프레임 애니메이션 전환 시간
	m_keepRunningTime = 0.1f;		// 달리기 잔여 시간
	m_walkSoundTime = 0.52f;		// 발걸음 사운드 타이머
	m_isOneStep = true;
	m_jumpTime = 1.f;				// 점프 애니메이션 지속 시간
	m_lateJumpTime = 0.1f;			// 착지 후 점프 유예 시간
	m_isJumpControl = false;
}

void PlayerComponent::Update(float _deltaTime)
{
	// 발걸음 스타일 랜덤 선택 (1~5)
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(1, 5);
	m_walkStyle = dis(gen);

	m_deltaTime = _deltaTime;
	m_position = m_transform->GetPosition();

	// 플레이어 주변 콜라이더 감지 (좌우 31.5 범위)
	m_myCollisionManager->DetectBoxToCollider(Vector2(m_position.x - 31.5f, -10000.f), Vector2(m_position.x + 31.5f, +10000.f), m_colliderVector);

	m_isKeyDown = false;

	if (m_changeObject)
	{
		// 토끼로 조작 중일 때 — 빈손 애니메이션
		if (GetRightDirection())
			m_animator->Play(playerFreeHand, true);
		else
			m_animator->Play(playerFreeHandFlip, true);
	}
	else
	{
		// 기본 Idle 애니메이션 (얼음 상태 여부에 따라 분기)
		if (GetRightDirection())
			m_animator->Play(m_isFreeze ? playerFreezeIdle : playerIdle, true);
		else
			m_animator->Play(m_isFreeze ? playerFreezeIdleFlip : playerIdleFlip, true);

		// 방향 전환
		if (m_myInputManager->IsKeyState(KEY::LEFT, KEY_STATE::TAP) && m_isInputOn)
			m_isRightDirection = false;
		if (m_myInputManager->IsKeyState(KEY::RIGHT, KEY_STATE::TAP) && m_isInputOn)
			m_isRightDirection = true;

		// 왼쪽 이동
		if (m_myInputManager->IsKeyState(KEY::LEFT, KEY_STATE::HOLD) && m_isInputOn && m_isRightDirection == false)
		{
			if (m_isOnIce)
			{
				// 빙판 위: 속도 직접 설정
				float yVel = m_isInverseGravity ? m_rigidBody->GetVelocity().y * -1 : m_rigidBody->GetVelocity().y;
				m_rigidBody->SetVelocity(Vector2(GetSpeed() * -1.f, yVel));
			}
			else
			{
				// 일반 이동: 역중력 시 y속도 반전 후 위치 직접 이동
				float yVel = m_isInverseGravity ? m_rigidBody->GetVelocity().y * -1 : m_rigidBody->GetVelocity().y;
				m_rigidBody->SetVelocity(Vector2(0.f, yVel));
				m_position.x -= GetSpeed() * _deltaTime;
			}

			m_animator->SetOffset(Vector2(20.f, 0.f));
			m_animator->Play(m_isFreeze ? playerFreezeWalkFlip : playerWalkFlip, true);

			if (m_isOnGround)
				WalkSound();

			m_isRun = true;
			m_keepRunningTime = 0.1f;
			SetRightDirection(false);
			m_isKeyDown = true;
		}
		else
		{
			m_animator->Reset(playerWalkFlip);
			m_animator->Reset(playerFreezeWalkFlip);
		}

		// 오른쪽 이동
		if (m_myInputManager->IsKeyState(KEY::RIGHT, KEY_STATE::HOLD) && m_isInputOn && m_isRightDirection == true)
		{
			m_animator->SetOffset(Vector2(-20.f, 0.f));

			if (m_isOnIce)
			{
				float yVel = m_isInverseGravity ? m_rigidBody->GetVelocity().y * -1.f : m_rigidBody->GetVelocity().y;
				m_rigidBody->SetVelocity(Vector2(GetSpeed(), yVel));
			}
			else
			{
				float yVel = m_isInverseGravity ? m_rigidBody->GetVelocity().y * -1.f : m_rigidBody->GetVelocity().y;
				m_rigidBody->SetVelocity(Vector2(0.f, yVel));
				m_position.x += GetSpeed() * _deltaTime;
			}

			m_animator->Play(m_isFreeze ? playerFreezeWalk : playerWalk, true);

			if (m_isOnGround)
				WalkSound();

			m_isRun = true;
			m_isKeyDown = true;
			m_keepRunningTime = 0.1f;
		}
		else
		{
			m_animator->Reset(playerWalk);
			m_animator->Reset(playerFreezeWalk);
		}

		// 아래키: 토끼로 전환
		if (m_myInputManager->IsKeyState(KEY::DOWN, KEY_STATE::TAP) && m_isInputOn && !m_isFreeze)
		{
			m_isRun = false;

			if (GetRightDirection())
				m_animator->Play(playerIdle, true);
			else
				m_animator->Play(playerIdleFlip, true);

			if (m_isOnGround)
			{
				SetChangeObject(true);
				m_rigidBody->SetFixed(true);

				// 인벤토리를 토끼에게 전달
				m_inventory->SetActive(false);
				m_inventory->PassItemList(m_rabbitInventory->GetInventory());
				m_rabbitInventory->SetActive(true);

				m_rabbit->GetComponent<BoxCollider>()->SetActive(true);
				GetCameraManager()->SetTargetObject(m_rabbit);
			}
		}

		// 점프
		if (m_myInputManager->IsKeyState(KEY::SPACE, KEY_STATE::TAP) && m_isInputOn)
		{
			if (m_jumpCount > 0 && m_isOnGround)
			{
				if (SoundManager::GetInstance()->isPlayingChannel(7) == false)
					SoundManager::GetInstance()->PlaySFXSound(L"jump", 1, 7);

				m_rigidBody->SetVelocity(Vector2(0.f, m_jumpPower));
				m_jumpCount--;
				m_prevPosition = m_position;
				m_isJump = true;
				m_isOnIce = 0;
			}
			m_isOnGround = false;
		}

		if (m_isJump)
			JumpAnimation();

		// R키: 역중력 전환 (지상에서만, 중력 전환 중이 아닐 때)
		if (m_isOnGround && GetGravitySwtchOn() == false)
		{
			if (m_myInputManager->IsKeyState(KEY::R, KEY_STATE::TAP) && m_isInputOn && !m_isFreeze && !GetStop())
			{
				m_playInverseAnimaitionTime = 0.f;
				m_isPlayInverseAnimaition = true;
				m_isPrevRightDirection = m_isRightDirection;

				// 콜라이더 오프셋 반전
				Vector2 offset = m_boxCollider->GetOffset();
				offset.y *= -1.f;
				m_boxCollider->SetOffset(offset);

				m_isInverseGravity = !m_isInverseGravity;
				SetGravitySwtchOn(true);
				m_isOnGround = false;
				m_jumpCount--;
				m_prevPosition = m_position;
				SetSpeed(150.f);

				if (m_actor)
					m_actor->SetInverse(m_isInverseGravity);
			}
		}

		InverseGravityAnimation(_deltaTime);
	}

	// 키 입력 없을 때 — 잔여 달리기 애니메이션 처리
	if (m_isKeyDown == false)
	{
		m_changeAnimationTime = 0.416f;
		KeepRunnig();
		WalkAnimationCheck();

		if (GetRightDirection() && m_isRun)
			m_animator->Play(playerWalkToIdle, true);
		else if (!GetRightDirection() && m_isRun)
			m_animator->Play(playerWalkToIdleFlip, true);
	}

	m_isChangeAnimationOn = true;

	// 역중력 RigidBody에 적용
	m_rigidBody->SetInverseGravity(m_isInverseGravity);

	// 트랜스폼에 최종 위치 반영
	m_transform->SetPosition(m_position);
}

void PlayerComponent::Render(D2DRenderer* _d2DRenderer) {}
void PlayerComponent::PostRender(D2DRenderer* _d2DRenderer) {}
void PlayerComponent::DebugRender(D2DRenderer* _d2DRenderer) {}

void PlayerComponent::OnCollisionEnter(const Collision& _collision)
{
	if (_collision.otherObject->GetObjectType() == OBJECT_TYPE::TILE)
	{
		// 중력 방향에 맞는 면에 착지했을 때
		if ((m_isInverseGravity && _collision.direct == COLLISION_DIRECT::UP) ||
			(m_isInverseGravity == false && _collision.direct == COLLISION_DIRECT::DOWN))
		{
			m_jumpTime = 1.f;
			m_isOnGround = true;
			SetGravitySwtchOn(false);
			m_jumpCount = 1;
			SetSpeed(300.f);
			m_isJump = false;
			m_isJumpControl = true;
		}
	}

	// 착지 방향에 따라 애니메이션 이름 세트 전환 (일반/역중력)
	if (_collision.direct == COLLISION_DIRECT::DOWN && m_isInverseGravity == false)
	{
		playerIdle = L"playerIdle";
		playerIdleFlip = L"playerIdleFlip";
		playerWalk = L"playerWalk";
		playerWalkFlip = L"playerWalkFlip";
		playerJump = L"playerJump";
		playerJumpFlip = L"playerJumpFlip";
		playerWalkToIdle = L"playerWalkToIdle";
		playerWalkToIdleFlip = L"playerWalkToIdleFlip";
		playerFreeHand = L"playerFreeHand";
		playerFreeHandFlip = L"playerFreeHandFlip";
		playerFreezeIdle = L"playerFreezeIdle";
		playerFreezeIdleFlip = L"playerFreezeIdleFlip";
		playerFreezeWalk = L"playerFreezeWalk";
		playerFreezeWalkFlip = L"playerFreezeWalkFlip";
		playerFreezeJump = L"playerFreezeJump";
		playerFreezeJumpFlip = L"playerFreezeJumpFlip";
		playerJumping = L"playerJumping";
		playerJumpingFlip = L"playerJumpingFlip";
		playerInverseGravity = L"playerInverseGravity";
		playerInverseGravityFlip = L"playerInverseGravityXFlip";
	}
	else if (_collision.direct == COLLISION_DIRECT::UP && m_isInverseGravity)
	{
		playerIdle = L"playerIdleInverse";
		playerIdleFlip = L"playerIdleFlipInverse";
		playerWalk = L"playerWalkInverse";
		playerWalkFlip = L"playerWalkFlipInverse";
		playerJump = L"playerJumpInverse";
		playerJumpFlip = L"playerJumpFlipInverse";
		playerWalkToIdle = L"playerWalkToIdleInverse";
		playerWalkToIdleFlip = L"playerWalkToIdleFlipInverse";
		playerFreeHand = L"playerFreeHandInverse";
		playerFreeHandFlip = L"playerFreeHandFlipInverse";
		playerFreezeIdle = L"playerFreezeIdleInverse";
		playerFreezeIdleFlip = L"playerFreezeIdleFlipInverse";
		playerFreezeWalk = L"playerFreezeWalkInverse";
		playerFreezeWalkFlip = L"playerFreezeWalkFlipInverse";
		playerFreezeJump = L"playerFreezeJumpInverse";
		playerFreezeJumpFlip = L"playerFreezeJumpFlipInverse";
		playerJumping = L"playerJumpingInverse";
		playerJumpingFlip = L"playerJumpingFlipInverse";
		playerInverseGravity = L"playerInverseGravityYFlip";
		playerInverseGravityFlip = L"playerInverseGravityXYFlip";
	}

	// 빙판 타일 접촉 시 미끄러움 카운트 증가
	if (_collision.otherObject->GetName() == "slideTile")
	{
		if (!_collision.otherObject->GetComponent<SlideTile>()->GetIsMelt())
			m_isOnIce++;
	}
}

void PlayerComponent::OnCollisionStay(const Collision& _collision)
{
	if (_collision.otherObject->GetObjectType() == OBJECT_TYPE::TILE)
	{
		if ((m_isInverseGravity && _collision.direct == COLLISION_DIRECT::UP) ||
			(m_isInverseGravity == false && _collision.direct == COLLISION_DIRECT::DOWN))
		{
			m_isJump = false;
			m_jumpCount = 1;
			m_isOnGround = true;
		}
	}
}

void PlayerComponent::OnCollisionExit(const Collision& _collision)
{
	if (_collision.otherObject->GetObjectType() == OBJECT_TYPE::TILE)
	{
		m_isJumpControl = true;
		m_isOnGround = false;
	}

	// 빙판 이탈 시 미끄러움 카운트 감소
	if (_collision.otherObject->GetName() == "slideTile")
	{
		if (!_collision.otherObject->GetComponent<SlideTile>()->GetIsMelt())
			m_isOnIce--;
	}

	// 깃털 아이템 이탈 시 물리 복원
	if (_collision.otherObject->GetName() == "feather")
	{
		m_rigidBody->SetFixed(false);
		m_rigidBody->SetMass(0.1f);
	}
}

void PlayerComponent::OnTriggerEnter(const Collision& _collision)
{
	// 고드름에 맞으면 얼음 상태 + HP 감소 + 중력 반전
	if (_collision.otherObject->GetName() == "icicle")
	{
		bool inverseGravity = _collision.otherObject->GetComponent<Icicle>()->IsInverseGravity();

		m_rigidBody->SetVelocity(Vector2::Zero);
		m_isFreeze = true;
		m_freezeEffect->Freeze();
		m_walkSoundTime = 1.4f;

		m_playerLife->DicreaseLife();
		if (m_playerLife->GetLife() <= 0)
		{
			// HP 0 → 현재 씬 재시작
			GetSceneManager()->RegisterNextScene(GetSceneManager()->GetCurrentScene()->GetSceneName());
		}

		if (m_isInverseGravity == inverseGravity)
			return;

		// 고드름 중력 방향과 다를 경우 중력 반전
		Vector2 offset = m_boxCollider->GetOffset();
		offset.y *= -1.f;
		m_boxCollider->SetOffset(offset);

		m_isInverseGravity = !m_isInverseGravity;
		SetGravitySwtchOn(true);
		m_isOnGround = false;
		m_jumpCount--;
		m_prevPosition = m_position;
	}
}

void PlayerComponent::InverseGravityAnimation(float dt)
{
	if (!m_isPlayInverseAnimaition)
		return;

	m_playInverseAnimaitionTime += dt;

	// 역중력 회전 애니메이션 종료 (0.8초 후)
	if (m_playInverseAnimaitionTime >= 0.8f)
	{
		m_animator->Reset(playerInverseGravity);
		m_animator->Reset(playerInverseGravityFlip);
		m_isPlayInverseAnimaition = false;

		// 애니메이션 이름 세트를 현재 중력 방향에 맞게 교체
		if (m_isInverseGravity == false)
		{
			playerIdle = L"playerIdle";
			playerIdleFlip = L"playerIdleFlip";
			playerWalk = L"playerWalk";
			playerWalkFlip = L"playerWalkFlip";
			playerJump = L"playerJump";
			playerJumpFlip = L"playerJumpFlip";
			playerWalkToIdle = L"playerWalkToIdle";
			playerWalkToIdleFlip = L"playerWalkToIdleFlip";
			playerFreeHand = L"playerFreeHand";
			playerFreeHandFlip = L"playerFreeHandFlip";
			playerFreezeIdle = L"playerFreezeIdle";
			playerFreezeIdleFlip = L"playerFreezeIdleFlip";
			playerFreezeWalk = L"playerFreezeWalk";
			playerFreezeWalkFlip = L"playerFreezeWalkFlip";
			playerFreezeJump = L"playerFreezeJump";
			playerFreezeJumpFlip = L"playerFreezeJumpFlip";
			playerJumping = L"playerJumping";
			playerJumpingFlip = L"playerJumpingFlip";
			playerInverseGravity = L"playerInverseGravity";
			playerInverseGravityFlip = L"playerInverseGravityXFlip";
		}
		else
		{
			playerIdle = L"playerIdleInverse";
			playerIdleFlip = L"playerIdleFlipInverse";
			playerWalk = L"playerWalkInverse";
			playerWalkFlip = L"playerWalkFlipInverse";
			playerJump = L"playerJumpInverse";
			playerJumpFlip = L"playerJumpFlipInverse";
			playerWalkToIdle = L"playerWalkToIdleInverse";
			playerWalkToIdleFlip = L"playerWalkToIdleFlipInverse";
			playerFreeHand = L"playerFreeHandInverse";
			playerFreeHandFlip = L"playerFreeHandFlipInverse";
			playerFreezeIdle = L"playerFreezeIdleInverse";
			playerFreezeIdleFlip = L"playerFreezeIdleFlipInverse";
			playerFreezeWalk = L"playerFreezeWalkInverse";
			playerFreezeWalkFlip = L"playerFreezeWalkFlipInverse";
			playerFreezeJump = L"playerFreezeJumpInverse";
			playerFreezeJumpFlip = L"playerFreezeJumpFlipInverse";
			playerJumping = L"playerJumpingInverse";
			playerJumpingFlip = L"playerJumpingFlipInverse";
			playerInverseGravity = L"playerInverseGravityYFlip";
			playerInverseGravityFlip = L"playerInverseGravityXYFlip";
		}
	}

	// 역중력 전환 중 회전 애니메이션 재생 + 카메라/사운드 처리
	if (GetGravitySwtchOn() == true)
	{
		if (m_isPrevRightDirection)
			m_animator->Play(playerInverseGravityFlip, true);
		else
			m_animator->Play(playerInverseGravity, true);

		m_myCameraManager->SetGravityMode(m_isInverseGravity ? -1 : 1);

		if (SoundManager::GetInstance()->isPlayingChannel(6) == false)
			SoundManager::GetInstance()->PlaySFXSound(L"inverse", 1, 6);
	}
	else
	{
		m_animator->Reset(playerInverseGravity);
		m_animator->Reset(playerInverseGravityFlip);
	}
}

void PlayerComponent::WalkAnimationCheck()
{
	// 카운트다운 후 걷기→Idle 전환 플래그 해제
	m_changeAnimationTime -= 1.f * m_deltaTime;
	if (m_changeAnimationTime < 0)
		m_isChangeAnimationOn = false;
}

void PlayerComponent::JumpAnimation()
{
	// 점프 초반(m_jumpTime > 0): 도약 애니메이션, 이후: 낙하 애니메이션
	m_jumpTime -= 1.f * m_deltaTime;

	if (GetRightDirection())
	{
		if (m_isFreeze)
			m_animator->Play(playerFreezeJump, true);
		else
			m_animator->Play(m_jumpTime < 0 ? playerJumping : playerJump, true);
	}
	else
	{
		if (m_isFreeze)
			m_animator->Play(playerFreezeJumpFlip, true);
		else
			m_animator->Play(m_jumpTime < 0 ? playerJumpingFlip : playerJumpFlip, true);
	}
}

void PlayerComponent::KeepRunnig()
{
	// 키를 뗀 후 짧은 시간 동안 달리기 상태 유지 (애니메이션 끊김 방지)
	m_keepRunningTime -= 1.f * m_deltaTime;
	if (m_keepRunningTime < 0)
		m_isRun = false;
}

void PlayerComponent::WalkSound()
{
	m_walkSoundTime -= 1.f * m_deltaTime;

	if (m_walkSoundTime < 0)
	{
		// 발걸음 사운드를 1~5 중 랜덤으로 재생
		wstring soundKey = L"girlWalkSound" + std::to_wstring(m_walkStyle);
		if (SoundManager::GetInstance()->isPlayingChannel(5) == false)
			SoundManager::GetInstance()->PlaySFXSound(soundKey.c_str(), 1, 5);

		// 얼음 상태일 때 발걸음 느리게
		m_walkSoundTime = m_isFreeze ? 1.0f : 0.52f;
	}
}
