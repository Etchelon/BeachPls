import QtQuick 2.2

import Logic 1.0

Flickable {
	id: root

	property bool active: true
	property Player selectedPlayer: null

	function reload()
	{
		if (selectedPlayer === null)
		{
			attack.rating = 0;
			pass.rating = 0;
			serve.rating = 0;
			set.rating = 0;
			reaction.rating = 0;
			discipline.rating = 0;

			return;
		}

		attack.rating = selectedPlayer.attack;
		pass.rating = selectedPlayer.pass;
		serve.rating = selectedPlayer.serve;
		set.rating = selectedPlayer.set;
		reaction.rating = selectedPlayer.reaction;
		discipline.rating = selectedPlayer.discipline;
	}

	contentHeight: column.height

	Column {
		id: column

		property bool compiled: attack.rating > 0 && pass.rating > 0 && serve.rating > 0 && set.rating > 0 && reaction.rating > 0 && discipline.rating > 0

		width: parent.width
		spacing: 20

		RatingBlock {
			id: attack

			width: parent.width

			skill: "Attacco"
			activeColor: "red"

			onRatingChanged: selectedPlayer.attack = rating;
		}

		RatingBlock {
			id: pass

			width: parent.width

			skill: "Ricezione"
			activeColor: "lightblue"

			onRatingChanged: selectedPlayer.pass = rating;
		}

		RatingBlock {
			id: serve

			width: parent.width

			skill: "Battuta"
			activeColor: "green"

			onRatingChanged: selectedPlayer.serve = rating;
		}

		RatingBlock {
			id: set

			width: parent.width

			skill: "Alzata"
			activeColor: "yellow"

			onRatingChanged: selectedPlayer.set = rating;
		}

		RatingBlock {
			id: reaction

			width: parent.width

			skill: "Reattività"
			activeColor: "purple"

			onRatingChanged: selectedPlayer.reaction = rating;
		}

		RatingBlock {
			id: discipline

			width: parent.width

			skill: "Disciplina"
			activeColor: "blue"

			onRatingChanged: selectedPlayer.discipline = rating;
		}
	}

	Item {
		id: indicator

		anchors {
			top: column.bottom
			topMargin: 20
			left: column.left
			right: column.right
		}

		height: 50
		visible: root.active

		Image {
			id: image

			anchors {
				left: parent.left
				top: parent.top
				bottom: parent.bottom
			}

			height: parent.height
			width: height

			source: column.compiled ? "qrc:///checkIcon.png" : "qrc:///warningIcon.png"
		}

		Text {
			anchors {
				left: image.right
				leftMargin: 10
				right: parent.right
				top: parent.top
				bottom: parent.bottom
			}

			text: column.compiled ? "Dati salvati" : "Compila tutti i dati"
			font.pixelSize: parent.height * 0.4
			verticalAlignment: Text.AlignVCenter
			horizontalAlignment: Text.AlignHCenter
			wrapMode: Text.Wrap
		}
	}

	Rectangle {
		id: shade

		anchors.fill: parent
		color: "black"
		opacity: 0.3
		visible: !root.active

		MouseArea {
			anchors.fill: parent
			onClicked: console.log("Touch well");
		}
	}

	Component.onCompleted: reload();
}
