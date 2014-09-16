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
			attack.rating = 1;
			pass.rating = 1;
			serve.rating = 1;
			set.rating = 1;
			reaction.rating = 1;
			discipline.rating = 1;

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

	Rectangle {
		id: shade

		anchors.fill: parent
		color: "black"
		opacity: 0.5
		visible: root.active

		MouseArea {
			anchors.fill: parent
			onClicked: console.log("Touch well");
		}
	}

	Component.onCompleted: reload();
}
