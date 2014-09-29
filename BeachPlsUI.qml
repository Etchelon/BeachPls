import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Window 2.0

import Logic 1.0

ApplicationWindow {
	id: app

	title: qsTr("Beach pls!")
	width: 480
	minimumWidth: width
	maximumWidth: width
	height: 800
	minimumHeight: height
	maximumHeight: height
	visible: true

	property bool ready: false

	Engine {
		id: engine

		Component.onCompleted:
		{
			app.ready = true;
			if (!username)
				loginPage.show();
		}
	}

	ListView{
		id: playerList

		anchors {
			left: parent.left
			top: parent.top
			bottom: parent.bottom
			margins: 10
		}

		model: engine.players
		delegate: PlayerListDelegate{ }

		width: 150

		Component.onCompleted: playerList.currentIndex = -1;
	}

	RatingList {
		id: ratingList

		active: playerList.currentIndex !== -1
		selectedPlayer: playerList.currentIndex === -1 ? null : playerList.model[playerList.currentIndex]
		onSelectedPlayerChanged:
		{
			reload();
			console.log("Selected player: " + selectedPlayer.name);
		}

		anchors {
			left: playerList.right
			leftMargin: 20
			top: parent.top
			topMargin: 10
			bottom: parent.bottom
			bottomMargin: 10
			right: parent.right
			rightMargin: 10
		}
	}

	Button {
		anchors {
			bottom: ratingList.bottom
			bottomMargin: 10
			horizontalCenter: ratingList.horizontalCenter
		}
		width: 100
		height: 50

		text: "Invia dati!"
		onClicked: engine.submit_to_server();
	}

	Rectangle {
		id: loginPage

		function show()
		{
			loginPage.y = 0;
		}

		function hide()
		{
			loginPage.y = height;
		}

		width: parent.width
		height: parent.height
		y: height
		Behavior on y {
			NumberAnimation { duration: app.ready ? 300 : 0 }
		}
		color: "white"

		MouseArea {
			anchors.fill: parent
			onClicked: console.log("Touch well");
		}

		TextField {
			id: usernameTI

			anchors.centerIn: parent
			width: parent.width * 0.75
			height: 50

			font.pixelSize: height * 0.65
		}

		TextField {
			id: passwordTI

			anchors {
				top: usernameTI.bottom
				topMargin: 10
				horizontalCenter: usernameTI.horizontalCenter
			}
			width: parent.width * 0.75
			height: 50

			font.pixelSize: height * 0.65
			echoMode: TextInput.Password
		}

		Button {
			anchors {
				top: passwordTI.bottom
				topMargin: 10
				horizontalCenter: passwordTI.horizontalCenter
			}
			width: 300
			height: 50

			text: "Login!"

			onClicked:
			{
				engine.username = usernameTI.text
				engine.password = passwordTI.text
				engine.login();
				loginPage.hide();
			}
		}
	}
}
