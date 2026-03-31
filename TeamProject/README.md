# Team Project — Paradise Bird

DirectX 11 기반 팀 프로젝트 게임.
도망다니며 미션을 수행하는 게임으로, 미니게임 요소가 포함되어 있습니다.

**팀 구성:** 기획 3 / 개발 6 / 아트 3
**개발 기간:** 약 1개월
**기술 스택:** C++, DirectX 11, HLSL, Assimp, PhysX

---

## 게임 플레이

![gameplay](Screenshots/gameplay.jpg)

![gauge](Screenshots/gauge.gif)

---

## 담당 작업

### 1. Shadow Map (그림자)

Deferred Rendering 파이프라인에서 동작하는 그림자 시스템 구현.
정적 메시와 스키닝된 캐릭터 메시 모두 지원합니다.

- `Shadow/ShadowMap.h` / `ShadowMap.cpp` — 뎁스맵 생성 및 바인딩
- `Shadow/DeferredShadowDepthPS.hlsl` — 뎁스값을 컬러로 출력하는 픽셀 셰이더
- `Shadow/DeferredShadowDepthVS.hlsl` — 본 팔레트(128개)를 활용한 스키닝 버텍스 셰이더
- `Shadow/DeferredShadowStaticVS.hlsl` — 정적 메시용 버텍스 셰이더

### 2. 캐릭터 애니메이션 (Assimp)

Assimp로 로딩한 애니메이션 데이터를 키프레임 보간하여 재생.

- `Animation/Animation.h` / `Animation.cpp`
- 키프레임 간 Position/Scaling은 Lerp, Rotation은 Slerp 보간 적용

### 3. UI 컴포넌트

| 파일 | 설명 |
|------|------|
| `UIGauge` | 게이지 바 UI |
| `UIHungry` | 배고픔 수치 표시 |
| `UISkill` | 스킬 슬롯 UI |
| `UIMinimap` / `UIMiniMapIcon` | 미니맵 및 아이콘 |
| `ScoreUI` | 점수 표시 UI |
