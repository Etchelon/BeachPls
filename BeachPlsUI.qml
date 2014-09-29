import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Window 2.0

import Logic 1.0

ApplicationWindow {
	title: qsTr("Beach pls!")
	width: 480
	minimumWidth: width
	maximumWidth: width
	height: 800
	minimumHeight: height
	maximumHeight: height
	visible: true

	Engine {
		id: engine
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

		text: "Submit!"
		onClicked: engine.submit_to_server();
	}

	Rectangle {
		id: loginPage

		width: parent.width
		height: parent.height
		Behavior on y {
			NumberAnimation { duration: 300 }
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
				loginPage.y = loginPage.height
			}
		}
	}
}
