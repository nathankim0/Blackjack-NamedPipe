# Blackjack-NamedPipe
고급프로그래밍 팀프로젝트 (ipc를 이용한 블랙잭)

1. 카드의 합이 21점 혹은 21점에 가장 가까운 사람이 승리한다.
2. 딜러의 첫 번째 카드를 제외한 모든 카드는 공개한다.
3. 에이스(A)는 자신에게 유리하게 1점 혹은 11점으로 계산하고, J,Q,K 는 10점으로 계산한다.
4. 처음 2장의 카드가 21점이면 '블랙잭'
5. 딜러와 플레이어 모두 '블랙잭'인 경우 무승부
6. 플레이어는 '블랙잭'이 아닌 경우 21점에 가까워지기 위해 추가 카드를 요구한다.
7. 플레이어의 판단에 따라 Hit(카드를 추가로 가져오는 것) 혹은 Stand(더 이상 카드를 받지 않는 것) 한다.
8. 카드의 합이 21점을 초과할 경우 0점으로 처리된다. (Bust)

소프트웨어 설계
---------------
<div>
<img src="https://user-images.githubusercontent.com/37360089/70676912-03758f00-1cd1-11ea-958d-05a9ceca7596.png"></img>
</div>


시나리오
--
① 서버 <br>
서버 실행 <br>
메인함수에서 파이프 생성 및 open <br>
카드덱 초기화 <br>


② 클라이언트 <br>
클라이언트 실행 (2개 이상 가능) <br>
파이프 open 및 서버에게 게임 시작을 알림 <br>

③ 서버 <br>
play_game_one 쓰레드를 클라이언트 수에 맞게 생성 <br>
게임 시작 <br>
카드를 클라이언트에게 2장, 플레이어가 볼 수 있는 딜러의 카드 한 장을 뽑아 클라이언트에게 전송 <br>

④ 클라이언트 <br>
서버에게서 카드 받음 <br>
클라이언트는 HIT(1)과 STAND중에 선택 (HIT: 한 장 더 받기, STAND: 그대로 진행) (파이프로 문자열 전달) <br>

⑤ 서버 <br>
파이프로 클라이언트에게 HIT / STAND 받음  <br>
HIT일 경우 카드 한 장을 더 뽑아서 클라이언트에게 전송 (STAND일 경우 게임을 진행) <br>
클라이언트와 서버간 HIT과 카드를 주고받다가 STAND가 나올 경우에 딜러의 패를 전송 <br>

⑥ 클라이언트 <br>
딜러의 패를 받고 플레이어와 딜러의 패를 합산하여 승패유무를 가리고 게임을 끝냄 <br>


역할분담
------
김진엽 : Pipe <br>
이진재 : Shared memory  <br>
박인효 : Message passing <br>


작업내용
--

 <table border="1">
	<th>날짜</th>
	<th>내용</th>
	<tr>
	    <td>10/30 </td>
	    <td>팀 결성</td>
	</tr>
	<tr>
	    <td>11/12</td>
	    <td>주제 선정 및 제안서 작성</td>
	</tr>
  <tr>
	    <td>11/17</td>
	    <td>블랙잭 구현 희의</td>
	</tr>
  <tr>
	    <td>11/20</td>
	    <td>블랙잭 구현 (간단한 순차적 프로그램, pipe 통신 1대1 구현)</td>
	</tr>
  <tr>
	    <td>11/23</td>
	    <td>블랙잭 구현 회의 및 코드 구현 완료, shared memory 구현 (단일 쓰레드, 1대1), message passing 및 pipe 구현 시작 (팀원 각자 프로그래밍)</td>
	</tr>
  <tr>
	    <td>11/25</td>
	    <td>1대n 다중 클라이언트 구현, shared memory 세마포어 구현, 다중 쓰레드 아이디어 회의</td>
	</tr>
  <tr>
	    <td>11/26</td>
	    <td>다중 쓰레드 아이디어 추가 회의, message passing / pipe 구현 중 세그멘테이션 오류 - 디버깅 완료</td>
	</tr>
  <tr>
	    <td>11/28</td>
	    <td>pipe 구현(단일 쓰레드, 1대1)</td>
	</tr>
  <tr>
	    <td>11/29</td>
	    <td>통화 회의, 다중 쓰레드 구현 시작, pipe, message passing 구현 중</td>
	</tr>
  <tr>
	    <td>11/30</td>
	    <td>shared memory 구현 완료 (다중 쓰레드 ,1대 n) pipe, message passing 1대 n 구현 시작</td>
	</tr>
  <tr>
	    <td>12/01</td>
	    <td>pipe 구현 완료 (다중 쓰레드, 1대 n)</td>
	</tr>
  <tr>
	    <td>12/02</td>
	    <td>message passing 구현 완료 (다중 쓰레드, 1대 n), 발표 자료 및 보고서 작성 완료</td>
	</tr>
  <tr>
	    <td>12/04</td>
	    <td>발표 (김진엽)</td>
	</tr>
    </table>
    

실행 영상 (C language, gcc compiler, ubuntu 18.04)
--
<div>
<img src="https://user-images.githubusercontent.com/37360089/71075915-35eb2480-21c8-11ea-9f95-273da3e3849a.gif"></img>
</div>
https://youtu.be/c5W7WsLBivI

