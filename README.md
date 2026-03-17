# NBC_CH4_HW9 - Multiplayer Turn-Based Game

언리얼 엔진 5(Unreal Engine 5)로 개발된 멀티플레이어 턴제 추리/퀴즈 게임 프로젝트입니다. 
서버와 클라이언트 간의 상태 동기화(Replication)와 턴 타이머 관리에 중점을 두고 설계되었습니다.

##  주요 기능 (Key Features)

* **멀티플레이어 네트워크 동기화**: `GameStateBase`를 통해 현재 턴을 진행 중인 플레이어, 남은 시간, 전체 시간 등의 정보가 동기화됩니다.
* **서버 기반의 권한 제어 (Authority)**: 게임의 핵심 로직(정답 설정, 턴 시작 및 강제 종료 등)은 철저히 서버(`HasAuthority()`)에서만 처리되도록 구현.
* **정교한 타이머 및 UI 시스템**: 
    * 클라이언트와 서버 모두 각자의 Timer를 돌려 UI를 부드럽게 갱신하며, Replicate 주기를 줄여 통신량을 감소시킬 수 있었습니다.
    * 서브시스템의 델리게이트를 통해 타이머 UI와 GameState의 시간관리를 디커플링하여 생성 시점과 무관하게 정상적인 UI출력이 가능합니다.
* **자동 턴 오버**: 주어진 시간(`TurnTotalTime`) 내에 정답을 맞히지 못하면, `TimeOut` 판정과 함께 서버에서 자동으로 다음 턴으로 전환시킵니다.

##  핵심 아키텍처 (Architecture)

### `AHW9_GameStateBase`
게임의 전반적인 턴 상태와 타이머를 관리하는 핵심 클래스입니다.

* **Replicated Properties**:
    * `TurnRemainTime`: 현재 턴의 남은 시간
    * `TurnPlayerName`: 현재 턴을 진행 중인 플레이어 이름
    * `TurnTotalTime`: 턴에 부여된 총 시간
* **주요 메서드**:
    * `StartTurnTimer()`: 서버에서 새로운 턴을 시작하고 타이머를 세팅합니다.
    * `OnRep_TurnRemainingTime()`: 클라이언트 측에서 타이머 동기화를 보정하고 UI를 업데이트합니다.
    * `UpdateTurnTimerUI()`: 지속적으로 델타 타임을 계산하여 남은 시간을 차감하고, 서버라면 0초가 될 시 `EndTurn`를 호출하여 턴을 강제 종료합니다.

### `UTimerSubsystem`
* 월드(World) 서브시스템으로 구현되어 생명주기 관리가 용이하며, GameState와 UI 컴포넌트 간의 결합도를 낮추는 이벤트 브로드캐스터 역할을 수행합니다.

##  플레이 방법 (How to Play)

1. 호스트가 세션을 생성하고 게임을 시작합니다.
2. 클라이언트들이 세션에 접속하여 게임에 참여합니다.
3. 각 플레이어는 자신에게 주어진 턴 시간(`TurnTotalTime`) 동안 정답(`AnswerString`)을 유추하여 입력해야 합니다. (최대 3회 시도 가능)
4. 시간을 초과하거나 정답을 맞히면 다음 플레이어에게 턴이 넘어갑니다.
