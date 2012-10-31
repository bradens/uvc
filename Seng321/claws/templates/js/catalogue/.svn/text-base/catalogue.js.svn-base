var catalogController = {
		storeEntry: function(data)
		{
			if (!data)
				return false;
			CommHandler.doPost(
					"/catalogue/storeEntry/", 
					data,
					function(res) { 
						if (res)
							window.location.reload(); 
					}
			);
		},
		
		getEntry: function(ID)
		{
			CommHandler.doPost(
					"/catalogue/getEntry/",
					{
						id:ID
					},
					function(res) {
						console.log(res);
					}
			);
		},
		
		viewItem: function(id)
		{
			var cItem = $("#catalogueItem" + id);
			$(".viewItemDlg .iprice").html(cItem.find(".itemPrice").html());
			$(".viewItemDlg .idescription").html(cItem.find(".itemDesc").html());
			$(".viewItemDlg img").attr("src", cItem.find(".catalogueItem").attr("imagesrc"));
			$(".viewItemDlg").dialog({
				width:'500',
				closeOnEscape: true,
				resizable:false,
				title: cItem.find(".itemName").html(),
				modal: true,
				dialogClass: "viewItemDlgWrap"
			});
		},
		
		addNewItem: function()
		{
			$(".addItemDlg").dialog({
				width:'637',
				height: '410',
				closeOnEscape: true,
				modal: true,
				dialogClass: "viewItemDlgWrap"
			});
		},
		
		finishAdd: function()
		{
			var cItem = $(".addItemDlg .itemInfo");
			data = {
				name: cItem.find(".addName").val(),
				description: cItem.find(".addDesc").val(),
				imageURL: cItem.find(".addImageUrl").val(),
				price: cItem.find(".addPrice").val()
			};
			this.storeEntry(data);
			$(".addItemDlg").dialog('close');
		},
		
		deleteItem: function(id)
		{
			if (!id)
				return false;
			CommHandler.doPost(
					"/catalogue/deleteEntry/", 
					{id: id},
					function(res) {
						if (res.id)
							$("#catalogueItem" + id).remove();
					}
			);
		}
}

$(document).ready(function(){
	$(".addImageUrl").keyup(function() {
		$(".addItemDlg img").attr("src", $(this).val());
	});
	$(".catalogueItem button.button").click(function(ev){
		catalogController.deleteItem($(this).closest(".catalogueItem").attr('storeid'));
		ev.stopPropagation();
		return false;
	});
});
