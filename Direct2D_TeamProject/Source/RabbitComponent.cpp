#include "UserScriptsPCH.h"
#include "RabbitComponent.h"
#include "PlayerComponent.h"
#include "Inventory.h"

// 토끼 캐릭터 컨트롤러 — 플레이어가 아래키로 전환해 조작하는 서브 캐릭터
RabbitComponent::RabbitComponent()
	:MonoBehaviour(typeid(this).name()), m_speed(0.f), m_transform(nullptr), m_animator(nullptr), m_isRightDirection(true), m_isOnGround(true)
	, m_rigidBody(nullptr), m_jumpCount(0), m_isInverseGravity(false), m_inventory(nullptr), m_isInputOn(false), m_playerInventory(nullptr)
	, m_playerTransform(nullptr), m_mainPlayer(nullptr), m_playerRigidBody(nullptr), m_actor(nullptr), m_isJump(false), m_myCameraManager(nullptr)
	, m_rabbitRangeX(0.f), m_rabbitRangeY(0.f), m_isGravitySwtchOn(false), m_deltaTime(0.f), m_isReturnPlayer(false), m_pointLight(nullptr)
	, m_isOneStep(false), m_walkSoundTime(0.f), returnPaticlePlayTime(0.f), m_paticleSystem(nullptr), m_isReturn(false), m_isFall(false)
	, brightness(0.f), m_isChangeObject(false)
{
}

RabbitComponent::~RabbitComponent()
{
}

Component* RabbitComponent::Clone()
{
	RabbitComponent* clone = new RabbitComponent();
	return clone;
}

void RabbitComponent::Start()
{
	// 필요한 컴포넌트 캐싱
	m_transform = GetComponent<Transform>();
	m_animator = GetComponent<Animator>();
	m_rigidBody = GetComponent<RigidBody>();
	m_actor = GetComponent<Actor>();
	m_boxColiider = GetComponent<BoxCollider>();

	// 인벤토리 초기화 (플레이어와 공유)
	m_playerInventory = m_mainPlayer->GetComponent<Inventory>();
	m_inventory = GetComponent<Inventory>();

	m_isInputOn = true;
	m_speed = 200.f;
	m_jumpCount = 1;
	m_mainPlayer = GetMainPlayer();

	m_playerTransform = m_mainPlayer->GetComponent<Transform>();
	m_playerRigidBody = m_mainPlayer->GetComponent<RigidBody>();
	m_myCameraManager = GetCameraManager();

	// 카메라 범위 밖 이탈 감지 기준값 (화면 절반 크기 - 여백)
	m_rabbitRangeX = 960 - 50;
	m_rabbitRangeY = 540 - 30;

	m_paticleSystem = GetComponent<ParticleSystem>();

	// 포인트 라이트 초기화 (복귀 이펙트용)
	m_pointLight = CreateComponent<PointLightRenderer>();
	brightness = 0.f;
	m_pointLight->SetIntensity(brightness);
}

void RabbitComponent::Update(float _deltaTime)
{
	m_deltaTime = _deltaTime;

	// 플레이어 상태 동기화
	Vector2 m_playerPosition = m_playerTransform->GetPosition();
	m_isChangeObject = m_mainPlayer->GetComponent<PlayerComponent>()->GetChangeObject();
	m_isInverseGravity = m_mainPlayer->GetComponent<PlayerComponent>()->GetIsInverseGravity();
	m_isGravitySwtchOn = m_mainPlayer->GetComponent<PlayerComponent>()->GetGravitySwtchOn();

	Vector2 position = m_transform->GetPosition();
	const InputManager* input = GetInputManager();
	m_cameraPosition = m_myCameraManager->GetCamera()->GetPosition();

	if (m_isChangeObject)
	{
		// 토끼 조작 활성화 상태
		m_myCameraManager->SetTargetObject(m_mainPlayer);
		m_animator->SetShow(true);

		// 기본 Idle 애니메이션
		if (m_isRightDirection)
			m_animator->Play(rabbitIdle, true);
		else
			m_animator->Play(rabbitIdleFlip, true);

		// 위키: 플레이어로 복귀 예약
		if (input->IsKeyState(KEY::UP, KEY_STATE::TAP) && m_isInputOn)
		{
			returnPaticlePlayTime = 0.5f;
			m_isReturnPlayer = true;
		}

		// 오른쪽 이동
		if (input->IsKeyState(KEY::RIGHT, KEY_STATE::HOLD) && m_isInputOn)
		{
			position.x += m_speed * _deltaTime;
			m_animator->Play(rabbitWalk, true);
			m_isRightDirection = true;

			if (m_isOnGround)
				WalkSound();
		}

		// 왼쪽 이동
		if (input->IsKeyState(KEY::LEFT, KEY_STATE::HOLD) && m_isInputOn)
		{
			position.x -= m_speed * _deltaTime;
			m_animator->Play(rabbitWalkFlip, true);
			m_isRightDirection = false;

			if (m_isOnGround)
				WalkSound();
		}

		// 점프
		if (input->IsKeyState(KEY::SPACE, KEY_STATE::TAP) && m_isInputOn)
		{
			if (m_jumpCount > 0 && m_isOnGround)
			{
				if (SoundManager::GetInstance()->isPlayingChannel(7) == false)
					SoundManager::GetInstance()->PlaySFXSound(L"jump", 1, 7);

				m_rigidBody->SetVelocity(Vector2(0.f, 650.f));
				m_jumpCount--;
				m_isJump = true;
			}
			m_isOnGround = false;
		}

		// 점프 중 애니메이션
		if (m_isOnGround == false)
		{
			if (m_isRightDirection)
				m_animator->Play(rabbitJump, true);
			else
				m_animator->Play(rabbitJumpFlip, true);
		}
	}
	else
	{
		// 토끼 비활성화 상태 — 플레이어 옆에 붙어서 따라다님
		m_animator->SetShow(false);
		if (m_mainPlayer->GetComponent<PlayerComponent>()->GetRightDirection())
			position = Vector2(m_playerPosition.x + 40.f, m_playerPosition.y);
		else
			position = Vector2(m_playerPosition.x - 40.f, m_playerPosition.y);
	}

	// 점프 없이 공중에 있으면 낙하 상태
	if (m_isOnGround == false && m_isJump == false)
		m_isFall = true;

	// 역중력 Actor 동기화
	if (m_actor)
		m_actor->SetInverse(m_isInverseGravity);

	// 역중력 RigidBody에 적용
	m_rigidBody->SetInverseGravity(m_isInverseGravity);

	// 중력 방향에 따라 콜라이더 오프셋 조정
	m_boxColiider->SetOffset(m_isInverseGravity ? Vector2(0.f, 12.5f) : Vector2(0.f, -12.5f));

	// 카메라 범위 초과 시 강제 복귀
	if (abs(position.x - m_cameraPosition.x) >= m_rabbitRangeX)
	{
		m_isReturnPlayer = true;
	}
	else if ((abs(position.y - m_cameraPosition.y) - 100.f >= m_rabbitRangeY) && m_isGravitySwtchOn == false && m_isReturnPlayer == false)
	{
		m_isReturnPlayer = true;
		m_prePosition = position;
	}
	else
	{
		m_transform->SetPosition(position);
	}

	if (m_isReturnPlayer)
	{
		m_isInputOn = false;
		ReturnPaticle();

		// 낙하 중이면 낙하 시작 위치 유지
		if (m_isFall == false)
			m_transform->SetPosition(position);
		else
			m_transform->SetPosition(m_prePosition);
	}

	if (m_isReturn)
		ReturnLight();
}

void RabbitComponent::Render(D2DRenderer* _d2DRenderer) {}
void RabbitComponent::PostRender(D2DRenderer* _d2DRenderer) {}
void RabbitComponent::DebugRender(D2DRenderer* _d2DRenderer) {}

void RabbitComponent::OnCollisionEnter(const Collision& _collision)
{
	if (_collision.otherObject->GetObjectType() == OBJECT_TYPE::TILE)
	{
		// 중력 방향에 맞는 면에 착지
		if ((m_isInverseGravity && _collision.direct == COLLISION_DIRECT::UP) ||
			(m_isInverseGravity == false && _collision.direct == COLLISION_DIRECT::DOWN))
		{
			m_isOnGround = true;
			m_jumpCount = 1;
			m_isJump = false;
		}
	}

	// 착지 방향에 따라 애니메이션 이름 세트 전환 (일반/역중력)
	if (_collision.direct == COLLISION_DIRECT::DOWN && m_isInverseGravity == false)
	{
		rabbitIdle = L"rabbitIdle";
		rabbitIdleFlip = L"rabbitIdleFlip";
		rabbitWalk = L"rabbitWalk";
		rabbitWalkFlip = L"rabbitWalkFlip";
		rabbitJump = L"rabbitJump";
		rabbitJumpFlip = L"rabbitJumpFlip";
	}
	else if (_collision.direct == COLLISION_DIRECT::UP && m_isInverseGravity)
	{
		rabbitIdle = L"rabbitIdleInverse";
		rabbitIdleFlip = L"rabbitIdleFlipInverse";
		rabbitWalk = L"rabbitWalkInverse";
		rabbitWalkFlip = L"rabbitWalkFlipInverse";
		rabbitJump = L"rabbitJumpInverse";
		rabbitJumpFlip = L"rabbitJumpFlipInverse";
	}
}

void RabbitComponent::OnCollisionStay(const Collision& _collision)
{
	if (_collision.otherObject->GetObjectType() == OBJECT_TYPE::TILE)
	{
		if ((m_isInverseGravity && _collision.direct == COLLISION_DIRECT::UP) ||
			(m_isInverseGravity == false && _collision.direct == COLLISION_DIRECT::DOWN))
		{
			m_isJump = false;
			m_jumpCount = 1;
			m_isOnGround = true;
			m_isFall = false;
		}
	}
}

void RabbitComponent::OnCollisionExit(const Collision& _collision)
{
	if (_collision.otherObject->GetObjectType() == OBJECT_TYPE::TILE)
		m_isOnGround = false;
}

void RabbitComponent::OnTriggerEnter(const Collision& _collision)
{
	// 고드름에 맞으면 플레이어로 강제 복귀
	if (_collision.otherObject->GetName() == "icicle")
		m_isReturnPlayer = true;
}

void RabbitComponent::ReturnToPlayer()
{
	returnPaticlePlayTime = 0.5f;

	// 조작권을 플레이어로 반환
	m_mainPlayer->GetComponent<PlayerComponent>()->SetChangeObject(false);

	// 인벤토리를 플레이어에게 반환
	m_inventory->SetActive(false);
	m_inventory->PassItemList(m_playerInventory->GetInventory());
	m_playerInventory->SetActive(true);

	m_playerRigidBody->SetFixed(false);
	m_playerRigidBody->SetMass(0.1f);

	GetComponent<BoxCollider>()->SetActive(false);
	GetCameraManager()->SetTargetObject(m_mainPlayer);

	m_pointLight->SetIntensity(brightness);
	m_isReturn = true;
}

void RabbitComponent::ReturnPaticle()
{
	// 복귀 이펙트: 포인트 라이트 밝기 증가 후 ReturnToPlayer 호출
	returnPaticlePlayTime -= 1.f * m_deltaTime;
	brightness += 1.f * m_deltaTime;
	m_pointLight->SetIntensity(brightness);
	m_pointLight->SetScale(Vector2(3.f, 3.f));

	if (SoundManager::GetInstance()->isPlayingChannel(11) == false)
		SoundManager::GetInstance()->PlaySFXSound(L"rabbitReturn", 1, 11);

	if (returnPaticlePlayTime < 0)
	{
		m_isReturnPlayer = false;
		m_isInputOn = true;
		ReturnToPlayer();
		m_animator->SetShow(false);
		m_paticleSystem->Play();
		brightness = 1.f;
	}
}

void RabbitComponent::ReturnLight()
{
	// 복귀 후 포인트 라이트 서서히 소멸
	brightness -= 1.f * m_deltaTime;
	m_pointLight->SetIntensity(brightness);

	if (brightness <= 0.f)
	{
		m_isReturn = false;
		brightness = 0.f;
	}
}

void RabbitComponent::WalkSound()
{
	m_walkSoundTime -= 1.f * m_deltaTime;

	if (m_walkSoundTime < 0)
	{
		// 두 가지 발걸음 사운드를 번갈아 재생
		if (m_isOneStep)
		{
			if (SoundManager::GetInstance()->isPlayingChannel(3) == false)
				SoundManager::GetInstance()->PlaySFXSound(L"dollWalkSound1", 1, 3);
			m_isOneStep = false;
		}
		else
		{
			if (SoundManager::GetInstance()->isPlayingChannel2(10) == false)
				SoundManager::GetInstance()->PlaySFXSound(L"dollWalkSound2", 2, 10);
			m_isOneStep = true;
		}
		m_walkSoundTime = 0.25f;
	}
}
