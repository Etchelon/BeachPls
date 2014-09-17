import QtQuick 2.2

Item {
	id: root

	property alias skill: label.text
	property alias rating: rating.rating
	property alias activeColor: rating.activeColor

//	color: "lightgray"
//	border.color: "black"

	implicitHeight: label.height + rating.height + rating.anchors.topMargin

	Text {
		id: label

		anchors {
			top: parent.top
			horizontalCenter: rating.horizontalCenter
		}

		text: "Skill"
		font.pixelSize: 20
	}

	RatingSlider {
		id: rating

		anchors {
			top: label.bottom
			topMargin: 10
			left: parent.left
			right: parent.right
		}
		height: root.width * 0.2
	}
}
