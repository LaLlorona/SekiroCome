# 구현 체크리스트 (combatsystem_base.md 기준)

> **완료 기준**: 각 항목 완료 후 실제 코드를 확인하여 `[ ]` → `[x]` 로 변경한다.
> **참고 설계 문서**: `docs/combatsystem_base.md`

---

## 현재 구현 완료 상태 (기존 코드 확인 기준)

- [x] HP 기본 구조 (`ACombatCharacter::CurrentHP / MaxHP`)
- [x] 공격 콤보 / 차지 공격 몽타주 재생
- [x] 공격 충돌 판정 (`DoAttackTrace` — sphere sweep)
- [x] 락온 컴포넌트 (`UCombatLockOnComponent`)
- [x] 가드 / 패링 상태머신 (Idle / Guard / PartialParry / PerfectParryRiposte)
- [x] PerfectParry 타이밍 창 (Guard 진입 후 `RiposteMinimumTimeWindow` 이내)
- [x] 리포스트 몽타주 재생 (`UCombatMontageSet`)
- [x] 공격 방향 열거형 (`EAttackDirection`: Up / Down / Left / Right)
- [x] 이동 입력 → 방향 변환 (`ECombatInputDirectionEnum`, `GetCombatInputDirection`)
- [x] 피격 해결 로직 (`CombatLogic::ResolveAttack` — 패링 여부 판별 후 데미지 적용)
- [x] AnimNotify 연결 (DoAttackTrace / CheckCombo / CheckChargedAttack)

---

## Phase 1 — 스태미나 시스템 (설계 §1)

> **우선 구현 이유**: 데미지 계산, MasterStrike 반격 조건 등 이후 모든 시스템이 스태미나에 의존한다.

- [x] `ACombatCharacter`에 `CurrentSP / MaxSP` 추가 (`MaxSP = 30 + 70 * HP/100`) — `UCombatVitalityComponent`(`CurrentHP/MaxHP/CurrentSP/MaxSP`)를 만들어 `ACombatCharacter`가 소유하는 방식으로 구현 (`VitalityComponent` 필드)
- [x] `ACombatEnemy`에 동일한 SP 필드 추가 — `VitalityComponent`(`UCombatVitalityComponent`) 소유, `BeginPlay`에서 `Initialize(CombatTuningDataTable, CombatTuningRowName)` 호출, `Tick`에서 `CustomUpdate` 명시 호출, `TakeDamage`도 `VitalityComponent::ApplyDamage` 경유로 전환 완료
- [x] 데미지 처리: SP > 0 이면 SP 우선 차감, 초과분 즉시 HP 전이 (`UCombatVitalityComponent::ApplyDamage`)
- [x] 스태미나 회복 틱 구현 (`SP_RegenPerSecond`, `UCombatVitalityComponent::CustomUpdate`에서 매 틱 처리 — 엔진 `TickComponent`가 아니라 `ACombatCharacter::Tick`에서 명시적으로 호출)
- [~] 회복 정지 조건 — 공격 / 가드 / 회피 / 점프 / 스프린트 시작 시 타이머 리셋 (`SP_RegenDelay`) — 플레이어 쪽 공격(`ComboAttack`/`ChargedAttack`)과 가드(`TryGuardStart`), AI 쪽 공격(`DoAIComboAttack`/`DoAIChargedAttack`) 모두 `VitalityComponent->OnRegenStopTimerBegin()` 연결 완료. 회피/점프/스프린트는 `ACombatCharacter`/`ACombatEnemy`에 해당 액션 자체가 아직 없어서 미적용 (해당 시스템 구현 시 같이 연결 필요)
- [x] 이동(걷기)은 회복 정지 조건에서 제외 (현재 `DoMove`는 스태미나 영향 없음 — 유지)
- [x] HP 변경 시 SP 최댓값 즉시 클램프 (`UCombatVitalityComponent::RecomputeMaxSP`, `ApplyDamage`/`ResetVitality`/`CustomUpdate`에서 호출)
- [x] DataTable 생성: `UCombatTuningDataTable`(`FCombatTuningRow`: `SP_RegenPerSecond`, `SP_RegenDelayInSecond`) + 전용 `UFactory`

---

## Phase 2 — 무기 데미지 타입 / 계산식 (설계 §1-2, 3-테이블 방식)

> **우선 구현 이유**: Phase 1 스태미나 시스템과 함께 데미지 파이프라인 완성.

- [x] 무기 데미지 테이블 (`FCombatWeaponDamageRow`: ThrustDamage/SlashDamage/BluntDamage) + `UCombatWeaponDamageDataTable` + 전용 `UFactory` (§1-2-1)
- [x] 공격 종류 배율 테이블 (`FCombatAttackTypeRow`: ThrustMultiplier/SlashMultiplier/BluntMultiplier/PriorityHealthDamageRatio) + `UCombatAttackTypeDataTable` + 전용 `UFactory` (§1-2-2, 좌/우/상/하 공격·Riposte·콤보피니셔·MasterStrike 10행 예정)
- [x] 방어구 방어력 테이블 (`FCombatDefenseRow`: ThrustDefense/SlashDefense/BluntDefense, RowName = 방어구 타입 ID) + `UCombatDefenseDataTable` + 전용 `UFactory` (§1-2-3) — 플레이어/적 공용으로 사용하도록 `FCombatEnemyDefenseRow`/`UCombatEnemyDefenseDataTable`에서 이름 변경 (기존 `DT_EnemyDefenseData.uasset` 호환을 위해 `DefaultEngine.ini`에 `CoreRedirects` 추가)
- [x] `FAttackData`에 공격 종류 RowName(`AttackTypeRowName`) 필드 추가 (무기 ID는 `ACombatCharacter`/`ACombatEnemy`가, 방어구 타입 ID는 `ACombatCharacter`/`ACombatEnemy`가 각각 직접 보유 — 장착 시스템은 추후 구현). `ICombatAttacker::GetWeaponID()` / `ICombatDamageable::GetArmorTypeID()` 인터페이스 게터 추가, `CombatLogic::ResolveAttack`에서 호출하여 값 확보 (실제 데미지 계산식에는 아직 미사용)
- [x] `CombatLogic::ResolveAttack`에 최종 데미지 계산식(§1-2-4) + 체력 우선 데미지 분리 적용(§1-2-5) 구현 — `CombatLogic::CalculateFinalDamage`가 `FDamageData(PriorityHealthDamage, RemainingDamage)`를 반환하도록 구현, `ICombatDamageable::ApplyDamage`/`UCombatVitalityComponent::ApplyDamage` 모두 `FDamageData`를 받도록 변경. `ACombatCharacter`/`ACombatEnemy`는 실제 반영 로직을 `ApplyDamageToVitality(const FDamageData&)` private 헬퍼로 분리하고, 엔진 표준 `TakeDamage(float,...)` 오버라이드는 이 프로젝트에서 아무도 호출하지 않아 완전히 제거함

---

## Phase 3 — 공격 방향 자동 전환 (설계 §3-1, §3-2)

> **우선 구현 이유**: 콤보 히스토리 버퍼와 MasterStrike 난이도 분기의 전제 조건.

- [x] 플레이어 "다음 공격 방향" 상태 변수 추가 (`EAttackDirection NextAttackDirection`)
- [x] 공격 성공 후 방향 자동 전환 테이블 구현 (§3-1: 우→좌, 좌→우, 아래→우, 위→좌)
- [x] 가드 성공(Block) 후 방향 자동 전환 테이블 구현 (§3-2: 우→좌, 좌→위, 아래→우, 위→우)
- [ ] `CombatAttackDirectionUI`에 `NextAttackDirection` 연동 (기존 UI 위젯 활용)

---

## Phase 4 — MasterStrike / PerfectBlock 고도화 (설계 §5)

> **우선 구현 이유**: 방향 자동 전환(Phase 3) 완료 후 방향-난이도 분기 구현 가능.

- [ ] MasterStrike 판정 창을 InGameTime 기준으로 변경 (Time Dilation 보정)
- [ ] 무기별 판정 창 DataTable 추가 (`MasterStrike_WindowStart`, `MasterStrike_WindowEnd`, `MasterStrike_ColliderActivation`)
- [ ] 방향 난이도 분기 구현 (§3-3: Down/Right = 자동흐름으로 MasterStrike 가능 / Up/Left = 수동 재조정 필요)
- [ ] MasterStrike 데미지: `UCombatAttackTypeDataTable`의 "MasterStrike" 행을 이용한 §1-2 통합 계산식 적용 (§5-4)
- [ ] 실패 분기 구현: 판정 창 이후 플레이어 공격 시 적 공격 캔슬 (§5-3)
- [ ] 실패 분기 구현: 일반 Block → 피해 감소, 적 콤보 유지 (§5-3)
- [ ] PerfectBlock 반격 창 (`PerfectBlock_CounterWindow`) DataTable 추가
- [ ] `PerfectBlock_GuaranteedHit` bool DataTable 추가

---

## Phase 5 — 콤보 커맨드 매칭 시스템 (설계 §4)

> **우선 구현 이유**: 방향 자동 전환(Phase 3) 완료 후 히스토리 버퍼 의미 있음.

- [ ] 공격 히스토리 버퍼 구조 추가 (`TArray<EAttackDirection> AttackHistoryBuffer`, 최대 길이 3)
- [ ] 공격 성공 시 버퍼에 방향 추가 + 패턴 매칭 검사
- [ ] 버퍼 초기화 조건 구현 (PerfectBlock 당함 / `Combo_BufferTimeout` 초과)
- [ ] DataTable 기반 콤보 패턴 정의 (`FCombatComboRow`: 패턴 배열 + 피니셔 몽타주 + 데미지값)
- [ ] 기본 패턴 3종 등록 (좌→우→좌 / 우→좌→우 / 아래→아래→우)
- [ ] 패턴 매칭 성공 시 피니셔 몽타주 재생
- [ ] 콤보 피니셔 데미지: `UCombatAttackTypeDataTable`의 "ComboFinisher" 행을 이용한 §1-2 통합 계산식 적용 — §4-5
- [ ] PerfectBlock 당하면 콤보 버퍼 초기화 (§3-5 확정 사항)
- [ ] 적 PerfectBlock 성공 시 반격 조건 (`Enemy_CounterStaminaThreshold` DataTable 상수)

---

## Phase 6 — 이동 시스템 (설계 §2)

> **우선 구현 이유**: 전투 느낌 완성을 위한 락온 연동 이동.

- [ ] 락온 시 좌우 입력 → 원형 스트레이프 이동 구현 (`LockOn_StrafeRadius`, `LockOn_AngularSpeed`)
- [ ] 스트레이프 반경 / 각속도 DataTable 추가
- [ ] AI 다수 포위 슬롯 시스템 (`Surround_MaxCloseSlots`, `Surround_CloseSlotRadius`, `Surround_SideAngle`)

---

## DataTable 전체 상수 목록 (설계 §7)

> 위 Phase 구현 완료 여부와 별개로, 하드코딩된 수치가 있으면 DataTable로 이동한다.

- [ ] `SP_RegenPerSecond`
- [ ] `SP_RegenDelay`
- [ ] `LockOn_StrafeRadius`
- [ ] `LockOn_AngularSpeed`
- [ ] `Surround_MaxCloseSlots`
- [ ] `Surround_CloseSlotRadius`
- [ ] `Surround_SideAngle`
- [ ] `Enemy_CounterStaminaThreshold`
- [ ] `Combo_BufferTimeout`
- [ ] `Combo_InputWindow`
- [ ] `MasterStrike_WindowStart` / `MasterStrike_WindowEnd` / `MasterStrike_ColliderActivation` (무기별)
- [ ] `PerfectBlock_CounterWindow`
- [ ] `PerfectBlock_GuaranteedHit`
- [x] 무기별 찌르기/베기/둔기 데미지 (`UCombatWeaponDamageDataTable`, §1-2-1)
- [x] 공격 종류별(좌/우/상/하 공격·Riposte·콤보피니셔·MasterStrike) 찌르기/베기/둔기 배율 + 체력 우선 비율 (`UCombatAttackTypeDataTable`, §1-2-2)
- [x] 방어구 타입별 찌르기/베기/둔기 방어력 (`UCombatDefenseDataTable`, §1-2-3, 플레이어/적 공용)
