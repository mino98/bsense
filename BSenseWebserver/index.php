<?php include("header.inc.php"); ?>

<div id="featured"  class="clearfloat">
	<div id="usual1" class="usual"> 
	
		<div id="tab1" class="tab">
		<a href="broadbandtest.php" class="takethetest">Take the test!</a>
		<img src="images/tab_1.png" class="tab_image" alt="Mapping Scottish Broadband"/>
		<h1>Mapping Broadband in Scotland</h1>
		<p>With public awareness and political will to enable universal broadband at a high, there is a great need to get <strong>the true picture of broadband access coverage and quality</strong> in Scotland and beyond. However, existing figures not only differ widely but also are incomplete, especially for Scotland.</p> 
		
		<p>We aim to fill this gap by taking a <strong>hybrid approach</strong> to combine broadband speed test measurement results from users with other publicly available broadband coverage data to create a more comprehensive and evolving broadband map.</p>

		<p><strong>You can help us</strong> in this effort by taking the following broadband speed test. It only takes a few minutes of your time. Very soon, you will not only be able to view the results of your test updating the map but also see the broadband situation in your neighbourhood.</p>

		<p>Note that this is purely a non-commercial effort <strong>to help improve the state of broadband in Scotland</strong>. In future, we plan to increase the scope of our effort beyond Scotland.</p>
		</div> 
		
<!--
		<div id="tab2" class="tab">
		<img src="images/tab_2.png" class="tab_image" alt="Mapping Scottish Broadband"/>
		<h1>Get accurate coverage data.</h1>
		<p><i>Coming soon...</i></p>
		</div> 
-->
		
		<div id="tab4" class="tab">
		<img src="images/tab_4.png" class="tab_image" alt="Mapping Scottish Broadband"/>
		<h1>Reliable Broadband Maps.</h1>
		<p><i>Coming soon...</i></p>
		</div> 
	
		<ul id="tabs" class="clearfloat"> 
			<li><a class="selected" href="#tab1">For Broadband Users</a></li> 
			<!-- <li><a href="#tab2">For ISPs</a></li>  -->
			<li><a href="#tab4">For Policy Makers</a></li> 
		</ul> 
	</div> 
	<script type="text/javascript"> 
	  $("#usual1").idTabs(function(id,list,set){ 
	    $("a",set).removeClass("selected") 
	    .filter("[@href='"+id+"']",set).addClass("selected"); 
	    for(i in list) 
	      $(list[i]).hide(); 
	    $(id).fadeIn(); 
	    return false; 
	  }); 
	</script>
	
</div>
<div id="content">
	<div class="col">
		<h2>About this initiative</h2>
		<?php include("about.inc.php"); ?>
	</div>

	<div class="col">
		<h2>Latest updates</h2>
		<?php include("news.inc.php"); ?>
	</div>
	
	<div class="col">
		<h2>Contact us</h2>
		<?php include("contact.inc.php"); ?>
	</div>
	<div class="clear"></div>
</div>

<?php include("footer.inc.php"); ?>
